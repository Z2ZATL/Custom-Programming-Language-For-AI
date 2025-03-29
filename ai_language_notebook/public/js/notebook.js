// AI Language Notebook IDE - main JavaScript

document.addEventListener('DOMContentLoaded', function() {
    // Global variables
    const notebookContainer = document.getElementById('notebook-container');
    const newCellBtn = document.getElementById('new-cell-btn');
    const runAllBtn = document.getElementById('run-all-btn');
    const saveBtn = document.getElementById('save-btn');
    const loadBtn = document.getElementById('load-btn');
    const exampleSelect = document.getElementById('example-select');
    const cellTemplate = document.getElementById('cell-template');
    const uploadBtn = document.getElementById('upload-btn');
    const fileUpload = document.getElementById('file-upload');
    const fileList = document.getElementById('file-list');
    const activityBarIcons = document.querySelectorAll('.activity-bar-icon');
    
    let cells = [];
    let cellCounter = 0;
    let datasets = [];
    let models = [];

    // Function to create a new cell
    function createCell(content = '') {
        cellCounter++;
        
        // Clone the template
        const cell = document.importNode(cellTemplate.content, true).querySelector('.cell');
        
        // Set cell number
        cell.querySelector('.cell-number').textContent = `[${cellCounter}]`;
        
        // Initialize CodeMirror editor
        const codeArea = cell.querySelector('.code-area');
        const editor = CodeMirror(codeArea, {
            value: content,
            mode: 'javascript', // Fallback to JavaScript until ailang is loaded
            theme: 'monokai',
            lineNumbers: true,
            autoCloseBrackets: true,
            indentUnit: 4,
            tabSize: 4,
            lineWrapping: true
        });
        
        // Set to JavaScript mode only - removing ailang mode attempt to avoid errors
        editor.setOption('mode', 'javascript');
        
        // Style for special AI Language keywords - basic syntax highlighting
        const keywords = [
            "start", "create", "load", "train", "evaluate", "ML", "DL", "RL",
            "model", "dataset", "neural_network", "agent", "environment"
        ];
        
        editor.on("change", function() {
            const doc = editor.getDoc();
            const text = doc.getValue();
            
            // Simple highlighting for known keywords
            keywords.forEach(keyword => {
                const regex = new RegExp("\\b" + keyword + "\\b", "g");
                let match;
                
                while ((match = regex.exec(text)) !== null) {
                    const start = {line: doc.posFromIndex(match.index).line, ch: doc.posFromIndex(match.index).ch};
                    const end = {line: start.line, ch: start.ch + keyword.length};
                    
                    // Mark keywords with special class
                    doc.markText(start, end, {className: 'ai-lang-keyword'});
                }
            });
        });
        
        // Store cell data
        const cellData = {
            id: `cell-${Date.now()}-${Math.floor(Math.random() * 1000)}`,
            element: cell,
            editor: editor,
            outputText: cell.querySelector('.output-text'),
            outputVisuals: cell.querySelector('.output-visuals')
        };
        
        cells.push(cellData);
        
        // Add event listeners
        cell.querySelector('.run-btn').addEventListener('click', () => {
            runCell(cellData);
        });
        
        cell.querySelector('.delete-btn').addEventListener('click', () => {
            deleteCell(cellData);
        });
        
        // Add to the notebook container
        notebookContainer.appendChild(cell);
        
        // Focus the editor
        setTimeout(() => {
            editor.focus();
            editor.refresh();
        }, 10);
        
        return cellData;
    }
    
    // Function to run a cell
    function runCell(cellData) {
        const code = cellData.editor.getValue();
        if (!code.trim()) return;
        
        // Clear previous outputs
        cellData.outputText.innerHTML = '';
        cellData.outputText.classList.remove('error');
        cellData.outputVisuals.innerHTML = '';
        
        // Show loading indicator
        const loadingIndicator = document.createElement('div');
        loadingIndicator.className = 'loading';
        cellData.outputText.appendChild(loadingIndicator);
        
        // Send to backend for execution
        executeAILangCode(code)
            .then(result => {
                cellData.outputText.innerHTML = '';
                cellData.outputText.textContent = result.text;
                
                // Handle visualizations if any
                if (result.visuals) {
                    displayVisualization(cellData.outputVisuals, result.visuals);
                }
            })
            .catch(error => {
                cellData.outputText.innerHTML = '';
                cellData.outputText.textContent = `Error: ${error.message}`;
                cellData.outputText.classList.add('error');
            });
    }
    
    // Function to delete a cell
    function deleteCell(cellData) {
        const index = cells.findIndex(cell => cell.id === cellData.id);
        if (index !== -1) {
            cells.splice(index, 1);
            notebookContainer.removeChild(cellData.element);
        }
    }
    
    // Function to run all cells
    function runAllCells() {
        cells.forEach(cell => {
            runCell(cell);
        });
    }
    
    // Function to save notebook
    function saveNotebook() {
        const notebookData = cells.map(cell => ({
            code: cell.editor.getValue()
        }));
        
        const notebookJSON = JSON.stringify(notebookData, null, 2);
        const blob = new Blob([notebookJSON], { type: 'application/json' });
        const url = URL.createObjectURL(blob);
        
        const a = document.createElement('a');
        a.href = url;
        a.download = 'ai_notebook.json';
        a.click();
        
        URL.revokeObjectURL(url);
    }
    
    // Function to load notebook
    function loadNotebook() {
        const input = document.createElement('input');
        input.type = 'file';
        input.accept = '.json';
        
        input.onchange = function(event) {
            const file = event.target.files[0];
            if (!file) return;
            
            const reader = new FileReader();
            reader.onload = function(e) {
                try {
                    const notebookData = JSON.parse(e.target.result);
                    
                    // Clear existing cells
                    notebookContainer.innerHTML = '';
                    cells = [];
                    cellCounter = 0;
                    
                    // Create cells from loaded data
                    notebookData.forEach(cellData => {
                        createCell(cellData.code);
                    });
                } catch (error) {
                    alert(`Error loading notebook: ${error.message}`);
                }
            };
            
            reader.readAsText(file);
        };
        
        input.click();
    }
    
    // Function to load example
    function loadExample(exampleType) {
        const exampleURL = `/api/examples/${exampleType}`;
        
        fetch(exampleURL)
            .then(response => response.json())
            .then(data => {
                // Clear existing cells
                notebookContainer.innerHTML = '';
                cells = [];
                cellCounter = 0;
                
                // Create cells for each line in the example
                data.lines.forEach(line => {
                    if (line.trim() && !line.startsWith('#')) {
                        createCell(line);
                    } else if (line.startsWith('#')) {
                        // Add comments as separate cells
                        createCell(line);
                    }
                });
            })
            .catch(error => {
                // If fetching example fails, try to use hardcoded examples
                console.error('Failed to fetch example from API, using hardcoded examples', error);
                loadHardcodedExample(exampleType);
            });
    }
    
    // Hardcoded examples as a fallback
    function loadHardcodedExample(exampleType) {
        let exampleCode = [];
        
        switch (exampleType) {
            case 'ml':
                exampleCode = [
                    '# เริ่มต้นโปรเจกต์ Machine Learning',
                    'start create ML',
                    '',
                    '# โหลดข้อมูล',
                    'load dataset "data/housing.csv" type "csv"',
                    '',
                    '# ทำความสะอาดข้อมูล',
                    'clean data with drop_na true fill_mean "numeric" handle_outliers true',
                    '',
                    '# แบ่งข้อมูลสำหรับการฝึกและทดสอบ',
                    'split data into train, test with ratio 0.8, 0.2',
                    '',
                    '# สร้างโมเดล Linear Regression',
                    'create model LinearRegression with normalize true fit_intercept true',
                    '',
                    '# ฝึกโมเดล',
                    'train model on train_data with epochs 100 learning_rate 0.01 batch_size 32'
                ];
                break;
                
            case 'dl':
                exampleCode = [
                    '# เริ่มต้นโปรเจกต์ Deep Learning',
                    'start create DL',
                    '',
                    '# โหลดข้อมูลรูปภาพ',
                    'load dataset "images/flowers/" type "image"',
                    '',
                    '# สร้างโครงข่ายประสาทเทียมแบบ CNN',
                    'create neural_network CNN',
                    '',
                    '# เพิ่ม layer ตามสถาปัตยกรรม',
                    'add layer convolutional filters 32 kernel_size 3x3 activation "relu"',
                    'add layer max_pooling size 2x2',
                    'add layer flatten',
                    'add layer dense nodes 128 activation "relu"',
                    'add layer dense nodes 5 activation "softmax"',
                    '',
                    '# คอมไพล์โมเดล',
                    'compile model with optimizer "adam" loss "categorical_crossentropy" metrics "accuracy"'
                ];
                break;
                
            case 'rl':
                exampleCode = [
                    '# เริ่มต้นโปรเจกต์ Reinforcement Learning',
                    'start create RL',
                    '',
                    '# สร้างสภาพแวดล้อม',
                    'create environment "CartPole-v1"',
                    '',
                    '# สร้าง agent แบบ DQN',
                    'create agent DQN',
                    '',
                    '# ตั้งค่าพารามิเตอร์ของ agent',
                    'configure agent with gamma 0.99 epsilon 1.0 epsilon_decay 0.995',
                    '',
                    '# ฝึก agent',
                    'train agent for episodes 1000 max_steps 500 batch_size 32'
                ];
                break;
                
            default:
                return;
        }
        
        // Clear existing cells
        notebookContainer.innerHTML = '';
        cells = [];
        cellCounter = 0;
        
        // Create cells from example code
        exampleCode.forEach(line => {
            if (line.trim()) {
                createCell(line);
            }
        });
    }
    
    // Function to display visualizations
    function displayVisualization(container, visualData) {
        if (visualData.type === 'image') {
            const img = document.createElement('img');
            img.src = visualData.data;
            img.alt = visualData.alt || 'Visualization';
            img.style.maxWidth = '100%';
            container.appendChild(img);
        } else if (visualData.type === 'html') {
            container.innerHTML = visualData.data;
        } else if (visualData.type === 'plot') {
            // Placeholder for plot visualization
            const plotContainer = document.createElement('div');
            plotContainer.className = 'plot-container';
            plotContainer.style.height = '300px';
            plotContainer.style.backgroundColor = '#f8f9fa';
            plotContainer.style.border = '1px solid #ddd';
            plotContainer.style.borderRadius = '4px';
            plotContainer.style.display = 'flex';
            plotContainer.style.alignItems = 'center';
            plotContainer.style.justifyContent = 'center';
            plotContainer.textContent = '[Plot visualization placeholder]';
            container.appendChild(plotContainer);
        }
    }
    
    // Function to execute AI Language code (sends to backend)
    async function executeAILangCode(code) {
        try {
            const response = await fetch('/api/execute', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ code })
            });
            
            if (!response.ok) {
                const errorData = await response.json();
                throw new Error(errorData.error || 'Unknown error executing code');
            }
            
            return await response.json();
        } catch (error) {
            // If the API call fails, simulate execution (for development purposes)
            console.warn('API call failed, using simulated execution', error);
            return simulateExecution(code);
        }
    }
    
    // Function to simulate execution (for when backend is not available)
    function simulateExecution(code) {
        return new Promise((resolve) => {
            setTimeout(() => {
                const lines = code.split('\n').filter(line => line.trim() && !line.startsWith('#'));
                let output = '';
                
                lines.forEach(line => {
                    const trimmedLine = line.trim();
                    output += `> ${trimmedLine}\n`;
                    
                    if (trimmedLine.startsWith('start create')) {
                        const type = trimmedLine.split(' ')[2];
                        output += `เริ่มต้นการสร้างโปรเจกต์ประเภท: ${type}\n`;
                        if (type === 'ML' || type === 'ml') {
                            output += 'เริ่มต้นโปรเจกต์ Machine Learning\n';
                        } else if (type === 'DL' || type === 'dl') {
                            output += 'เริ่มต้นโปรเจกต์ Deep Learning\n';
                        } else if (type === 'RL' || type === 'rl') {
                            output += 'เริ่มต้นโปรเจกต์ Reinforcement Learning\n';
                        }
                    } else if (trimmedLine.startsWith('load dataset')) {
                        const match = trimmedLine.match(/"([^"]+)"/);
                        if (match) {
                            output += `กำลังโหลดข้อมูลจากไฟล์: ${match[1]}\n`;
                        }
                    } else if (trimmedLine.startsWith('train model')) {
                        output += 'กำลังฝึกโมเดล\n';
                        if (trimmedLine.includes('epochs')) {
                            const match = trimmedLine.match(/epochs (\d+)/);
                            if (match) {
                                output += `พารามิเตอร์: epochs = ${match[1]}\n`;
                            }
                        }
                    } else if (trimmedLine.startsWith('visualize')) {
                        const parts = trimmedLine.split(' ');
                        const dataType = parts[1] || 'data';
                        output += `กำลังแสดงภาพข้อมูล: ${dataType}\n`;
                    } else if (trimmedLine.startsWith('create model') || trimmedLine.startsWith('create neural_network')) {
                        output += 'กำลังสร้างโมเดล\n';
                    } else if (trimmedLine.startsWith('evaluate')) {
                        output += 'กำลังประเมินโมเดล\n';
                    } else if (trimmedLine.startsWith('show')) {
                        const metric = trimmedLine.split(' ')[1];
                        output += `กำลังแสดงเมตริก: ${metric}\n`;
                    } else if (trimmedLine.startsWith('create agent')) {
                        output += 'กำลังสร้าง agent สำหรับ Reinforcement Learning\n';
                    } else if (trimmedLine.startsWith('train agent')) {
                        output += 'กำลังฝึก agent\n';
                        if (trimmedLine.includes('episodes')) {
                            const match = trimmedLine.match(/episodes (\d+)/);
                            if (match) {
                                output += `จำนวน episodes: ${match[1]}\n`;
                            }
                        }
                    } else if (trimmedLine.startsWith('add layer')) {
                        output += 'กำลังเพิ่ม layer\n';
                    } else if (trimmedLine.startsWith('plot')) {
                        const metric = trimmedLine.split(' ')[1];
                        output += `กำลังพล็อตกราฟ: ${metric}\n`;
                    }
                });
                
                resolve({
                    text: output,
                    visuals: code.includes('visualize') ? {
                        type: 'plot',
                        data: null
                    } : null
                });
            }, 500); // Simulate a delay
        });
    }
    
    // Removed documentation functions
    // Now we implement proper activity bar functionality
    function handleActivityBarClick(icon) {
        const view = icon.getAttribute('data-view');
        
        // Update active state
        document.querySelectorAll('.activity-bar-icon').forEach(i => i.classList.remove('active'));
        icon.classList.add('active');
        
        console.log(`Switching to view: ${view}`);
        
        // For now, we'll just show/hide relevant panels based on selected view
        switch(view) {
            case 'explorer':
                // Show explorer panel
                document.querySelector('.explorer').style.display = 'flex';
                break;
                
            case 'search':
                // Search functionality to be implemented
                break;
                
            case 'models':
                // AI Models view to be implemented
                break;
                
            case 'settings':
                // Settings view to be implemented
                break;
        }
    }
    
    // File management functions
    
    // Function to upload dataset
    async function uploadDataset(file) {
        const formData = new FormData();
        formData.append('file', file);
        formData.append('name', file.name);
        formData.append('description', `Uploaded on ${new Date().toLocaleString()}`);
        
        try {
            const response = await fetch('/api/datasets/upload', {
                method: 'POST',
                body: formData
            });
            
            if (!response.ok) {
                const errorData = await response.json();
                throw new Error(errorData.error || 'Upload failed');
            }
            
            const result = await response.json();
            
            // Add to datasets list
            datasets.push(result.dataset);
            
            // Update file list display
            updateFileList();
            
            // Show success notification
            showNotification('Dataset uploaded successfully', 'success');
            
            return result.dataset;
        } catch (error) {
            showNotification(`Upload failed: ${error.message}`, 'error');
            throw error;
        }
    }
    
    // Function to load datasets from API
    async function loadDatasets() {
        try {
            const response = await fetch('/api/datasets');
            
            if (!response.ok) {
                throw new Error('Failed to load datasets');
            }
            
            datasets = await response.json();
            updateFileList();
        } catch (error) {
            console.error('Error loading datasets:', error);
            showNotification('Failed to load datasets', 'error');
        }
    }
    
    // Function to update file list display
    function updateFileList() {
        fileList.innerHTML = '';
        
        if (datasets.length === 0) {
            const emptyState = document.createElement('div');
            emptyState.className = 'empty-state';
            emptyState.textContent = 'อัพโหลดไฟล์ข้อมูลเพื่อเริ่มต้น';
            fileList.appendChild(emptyState);
            return;
        }
        
        // Group files by type
        const fileGroups = {
            'csv': [],
            'json': [],
            'image': [],
            'text': [],
            'other': []
        };
        
        datasets.forEach(dataset => {
            const type = dataset.file_type;
            if (fileGroups[type]) {
                fileGroups[type].push(dataset);
            } else {
                fileGroups.other.push(dataset);
            }
        });
        
        // Create group elements
        for (const [type, files] of Object.entries(fileGroups)) {
            if (files.length === 0) continue;
            
            const groupEl = document.createElement('div');
            groupEl.className = 'file-group';
            
            const groupHeader = document.createElement('div');
            groupHeader.className = 'file-group-header';
            groupHeader.textContent = type.toUpperCase();
            groupEl.appendChild(groupHeader);
            
            const filesList = document.createElement('div');
            filesList.className = 'files-list';
            
            files.forEach(file => {
                const fileEl = document.createElement('div');
                fileEl.className = 'file-item';
                fileEl.innerHTML = `
                    <span class="file-icon">
                        <i class="fas fa-${getFileIcon(file.file_type)}"></i>
                    </span>
                    <span class="file-name">${file.name}</span>
                `;
                
                // Click to insert into cell
                fileEl.addEventListener('click', () => {
                    insertDatasetReference(file);
                });
                
                filesList.appendChild(fileEl);
            });
            
            groupEl.appendChild(filesList);
            fileList.appendChild(groupEl);
        }
    }
    
    // Get appropriate icon for file type
    function getFileIcon(fileType) {
        switch (fileType) {
            case 'csv':
                return 'table';
            case 'json':
                return 'code';
            case 'image':
            case 'png':
            case 'jpg':
            case 'jpeg':
                return 'image';
            case 'txt':
                return 'file-alt';
            default:
                return 'file';
        }
    }
    
    // Insert dataset reference into current/new cell
    function insertDatasetReference(dataset) {
        const loadCommand = `load dataset "${dataset.name}" type "${dataset.file_type}"`;
        
        // If no cells, create one
        if (cells.length === 0) {
            createCell(loadCommand);
        } else {
            // Try to find an empty cell
            const emptyCell = cells.find(cell => cell.editor.getValue().trim() === '');
            
            if (emptyCell) {
                emptyCell.editor.setValue(loadCommand);
            } else {
                // Create a new cell
                createCell(loadCommand);
            }
        }
    }
    
    // Show notification
    function showNotification(message, type = 'info') {
        // If a notification container doesn't exist, create one
        let notificationContainer = document.querySelector('.notification-container');
        if (!notificationContainer) {
            notificationContainer = document.createElement('div');
            notificationContainer.className = 'notification-container';
            document.body.appendChild(notificationContainer);
        }
        
        const notification = document.createElement('div');
        notification.className = `notification ${type}`;
        notification.textContent = message;
        
        notificationContainer.appendChild(notification);
        
        // Auto remove after 5 seconds
        setTimeout(() => {
            notification.classList.add('fade-out');
            setTimeout(() => {
                notificationContainer.removeChild(notification);
            }, 300);
        }, 5000);
    }
    
    // Activity bar functionality
    function handleActivityBarClick(icon) {
        const view = icon.getAttribute('data-view');
        
        // Update active state
        document.querySelectorAll('.activity-bar-icon').forEach(i => i.classList.remove('active'));
        icon.classList.add('active');
        
        // Handle view switching here
        // This is a placeholder for future functionality
        console.log(`Switching to view: ${view}`);
        
        // For now, we'll just show/hide relevant panels
        switch(view) {
            case 'explorer':
                // Show explorer panel
                document.querySelector('.explorer').style.display = 'flex';
                break;
            // Documentation view removed
            // Add other views as needed
        }
    }
    
    // Add event listeners
    newCellBtn.addEventListener('click', () => createCell());
    runAllBtn.addEventListener('click', runAllCells);
    saveBtn.addEventListener('click', saveNotebook);
    loadBtn.addEventListener('click', loadNotebook);
    
    exampleSelect.addEventListener('change', function() {
        const value = this.value;
        if (value) {
            loadExample(value);
            this.value = ''; // Reset select
        }
    });
    
    // File upload event listeners
    uploadBtn.addEventListener('click', () => {
        fileUpload.click();
    });
    
    fileUpload.addEventListener('change', async (event) => {
        const files = event.target.files;
        if (files.length > 0) {
            try {
                await uploadDataset(files[0]);
                fileUpload.value = ''; // Reset file input
            } catch (error) {
                console.error('Upload error:', error);
            }
        }
    });
    
    // Activity bar event listeners
    activityBarIcons.forEach(icon => {
        icon.addEventListener('click', () => handleActivityBarClick(icon));
    });
    
    // Initialize notification system
    // Create notification container
    const notificationContainer = document.createElement('div');
    notificationContainer.className = 'notification-container';
    document.body.appendChild(notificationContainer);
    
    // Create an initial cell
    createCell('# เริ่มต้นเขียนโค้ด AI Language ที่นี่\n\n');
    
    // Style tweaks for Codemirror to match VS Code theme
    document.querySelectorAll('.CodeMirror').forEach(editor => {
        editor.style.backgroundColor = 'var(--cell-bg)';
    });
    
    // Load existing datasets
    loadDatasets();
});