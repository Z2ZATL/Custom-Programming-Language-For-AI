/**
 * Google Colab-style Notebook UI Implementation
 * This script handles the interactive notebook functionality including:
 * - Menu interactions
 * - Cell creation, editing, and execution
 * - File operations
 * - Runtime management
 */

document.addEventListener('DOMContentLoaded', function() {
    // DOM Elements
    const notebookContainer = document.getElementById('notebook-container');
    const notebookTitle = document.getElementById('notebook-title');
    const renameButton = document.getElementById('rename-button');
    const renameModal = document.getElementById('rename-modal');
    const newNotebookNameInput = document.getElementById('new-notebook-name');
    const confirmRenameButton = document.getElementById('confirm-rename');
    const cancelRenameButton = document.getElementById('cancel-rename');
    const closeModalButtons = document.querySelectorAll('.close');
    const addCodeCellButton = document.getElementById('add-code-cell');
    const addTextCellButton = document.getElementById('add-text-cell');
    const runCellButton = document.getElementById('run-cell');
    const runAllButton = document.getElementById('run-all');
    const saveNotebookButton = document.getElementById('save-notebook');
    const keyboardShortcutsButton = document.getElementById('keyboard-shortcuts');
    const keyboardShortcutsModal = document.getElementById('keyboard-shortcuts-modal');
    const notification = document.getElementById('notification');
    const notificationMessage = document.getElementById('notification-message');
    const closeNotificationButton = document.getElementById('close-notification');
    const panelTabs = document.querySelectorAll('.panel-tab');

    // State Variables
    let cells = [];
    let cellCounter = 0;
    let activeCell = null;
    let notebookFileName = 'Untitled.ipynb';

    // Initialize the notebook
    initializeMenu();
    initializePanels();
    createDefaultCell();

    // === File Menu Functions ===

    // Function to rename notebook
    function renameNotebook() {
        newNotebookNameInput.value = notebookFileName.replace('.ipynb', '');
        showModal(renameModal);
        newNotebookNameInput.focus();
    }

    // Function to confirm rename
    function confirmRename() {
        const newName = newNotebookNameInput.value.trim();
        if (newName) {
            notebookFileName = newName.endsWith('.ipynb') ? newName : `${newName}.ipynb`;
            notebookTitle.textContent = notebookFileName;
            hideModal(renameModal);
            showNotification(`ชื่อสมุดบันทึกถูกเปลี่ยนเป็น "${notebookFileName}"`, 'success');
        }
    }

    // Function to save notebook
    function saveNotebook() {
        // In a real implementation, this would send data to the server
        // For now, we'll just show a notification
        showNotification('บันทึกสมุดบันทึกเรียบร้อยแล้ว', 'success');
        
        // Simulate saving to localStorage for demo purposes
        const notebookData = {
            fileName: notebookFileName,
            cells: cells.map(cell => {
                return {
                    id: cell.id,
                    type: cell.type,
                    content: cell.editor ? cell.editor.getValue() : '',
                    output: cell.outputElement ? cell.outputElement.textContent : ''
                };
            })
        };
        
        localStorage.setItem('notebook_data', JSON.stringify(notebookData));
    }

    // Function to load notebook
    function loadNotebook() {
        // In a real implementation, this would load data from the server
        // For demo purposes, we'll load from localStorage if available
        const savedData = localStorage.getItem('notebook_data');
        
        if (savedData) {
            try {
                const notebookData = JSON.parse(savedData);
                
                // Clear existing cells
                notebookContainer.innerHTML = '';
                cells = [];
                cellCounter = 0;
                
                // Update notebook title
                notebookFileName = notebookData.fileName;
                notebookTitle.textContent = notebookFileName;
                
                // Create cells from saved data
                notebookData.cells.forEach(cellData => {
                    const cell = createCell(cellData.type, cellData.content);
                    if (cellData.output && cell.outputElement) {
                        cell.outputElement.textContent = cellData.output;
                        cell.outputElement.style.display = 'block';
                    }
                });
                
                showNotification('โหลดสมุดบันทึกเรียบร้อยแล้ว', 'success');
            } catch (error) {
                console.error('Error loading notebook:', error);
                showNotification('เกิดข้อผิดพลาดในการโหลดสมุดบันทึก', 'error');
            }
        } else {
            showNotification('ไม่พบสมุดบันทึกที่บันทึกไว้', 'info');
        }
    }

    // Function to download notebook
    function downloadNotebook(format = 'ipynb') {
        // Create notebook data structure
        const notebookData = {
            metadata: {
                kernelspec: {
                    display_name: "Python 3",
                    language: "python",
                    name: "python3"
                },
                language_info: {
                    name: "python",
                    version: "3.8"
                },
                title: notebookFileName
            },
            nbformat: 4,
            nbformat_minor: 5,
            cells: cells.map(cell => {
                return {
                    cell_type: cell.type === 'code' ? 'code' : 'markdown',
                    metadata: {},
                    source: cell.editor ? cell.editor.getValue().split('\n') : [],
                    outputs: cell.outputElement && cell.outputElement.textContent ? 
                        [{
                            output_type: "display_data",
                            data: {
                                "text/plain": [cell.outputElement.textContent]
                            }
                        }] : []
                };
            })
        };
        
        let content, fileName, mimeType;
        
        // Format specific conversions
        if (format === 'ipynb') {
            content = JSON.stringify(notebookData, null, 2);
            fileName = notebookFileName;
            mimeType = 'application/json';
        } else if (format === 'py') {
            // Convert to Python (.py) format
            content = cells.map(cell => {
                if (cell.type === 'code') {
                    return cell.editor ? cell.editor.getValue() : '';
                } else {
                    // Convert markdown to Python comments
                    const markdown = cell.editor ? cell.editor.getValue() : '';
                    return markdown.split('\n').map(line => `# ${line}`).join('\n');
                }
            }).join('\n\n');
            fileName = notebookFileName.replace('.ipynb', '.py');
            mimeType = 'text/plain';
        } else if (format === 'html') {
            // Simple HTML conversion
            content = `<!DOCTYPE html>
<html>
<head>
    <title>${notebookFileName}</title>
    <style>
        body { font-family: Arial, sans-serif; line-height: 1.6; padding: 20px; max-width: 800px; margin: 0 auto; }
        .cell { margin-bottom: 20px; }
        .code { font-family: monospace; background-color: #f5f5f5; padding: 10px; border-radius: 4px; }
        .markdown { }
        .output { background-color: #f9f9f9; padding: 10px; border-left: 3px solid #ccc; margin-top: 5px; }
    </style>
</head>
<body>
    <h1>${notebookFileName}</h1>
    ${cells.map(cell => {
        const content = cell.editor ? cell.editor.getValue() : '';
        const output = cell.outputElement ? cell.outputElement.textContent : '';
        
        if (cell.type === 'code') {
            return `<div class="cell">
                <div class="code">${content.replace(/</g, '&lt;').replace(/>/g, '&gt;')}</div>
                ${output ? `<div class="output">${output.replace(/</g, '&lt;').replace(/>/g, '&gt;')}</div>` : ''}
            </div>`;
        } else {
            // Basic markdown processing (in a real app, use a proper markdown parser)
            let html = content.replace(/</g, '&lt;').replace(/>/g, '&gt;');
            html = html.replace(/^# (.+)$/gm, '<h1>$1</h1>');
            html = html.replace(/^## (.+)$/gm, '<h2>$1</h2>');
            html = html.replace(/^### (.+)$/gm, '<h3>$1</h3>');
            html = html.replace(/\*\*(.+?)\*\*/g, '<strong>$1</strong>');
            html = html.replace(/\*(.+?)\*/g, '<em>$1</em>');
            
            return `<div class="cell">
                <div class="markdown">${html}</div>
            </div>`;
        }
    }).join('\n')}
</body>
</html>`;
            fileName = notebookFileName.replace('.ipynb', '.html');
            mimeType = 'text/html';
        }
        
        // Create download link
        const blob = new Blob([content], { type: mimeType });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = fileName;
        a.click();
        
        // Cleanup
        setTimeout(() => URL.revokeObjectURL(url), 100);
        
        showNotification(`ดาวน์โหลด ${fileName} เรียบร้อยแล้ว`, 'success');
    }

    // === Cell Management Functions ===

    // Function to create a new cell
    function createCell(type = 'code', content = '') {
        const cell = document.createElement('div');
        cell.className = 'cell';
        cell.dataset.id = cellCounter++;
        cell.dataset.type = type;
        
        // Create cell toolbar
        const cellToolbar = document.createElement('div');
        cellToolbar.className = 'cell-toolbar';
        
        const cellType = document.createElement('div');
        cellType.className = 'cell-type';
        cellType.textContent = type === 'code' ? 'Code' : 'Markdown';
        cellToolbar.appendChild(cellType);
        
        const cellActions = document.createElement('div');
        cellActions.className = 'cell-actions';
        
        // Add run button for code cells
        if (type === 'code') {
            const runButton = document.createElement('button');
            runButton.className = 'cell-action-button run-cell';
            runButton.innerHTML = '<i class="fas fa-play"></i>';
            runButton.title = 'Run cell';
            runButton.addEventListener('click', () => runCell(cell));
            cellActions.appendChild(runButton);
        }
        
        // Add common cell action buttons
        const deleteButton = document.createElement('button');
        deleteButton.className = 'cell-action-button delete-cell';
        deleteButton.innerHTML = '<i class="fas fa-trash"></i>';
        deleteButton.title = 'Delete cell';
        deleteButton.addEventListener('click', () => deleteCell(cell));
        cellActions.appendChild(deleteButton);
        
        cellToolbar.appendChild(cellActions);
        cell.appendChild(cellToolbar);
        
        // Create cell editor
        const cellEditor = document.createElement('div');
        cellEditor.className = 'cell-editor';
        cell.appendChild(cellEditor);
        
        // Initialize CodeMirror editor
        const mode = type === 'code' ? 'python' : 'markdown';
        const editor = CodeMirror(cellEditor, {
            value: content,
            mode: mode,
            theme: 'monokai',
            lineNumbers: type === 'code',
            lineWrapping: true,
            matchBrackets: true,
            autoCloseBrackets: true,
            indentUnit: 4,
            tabSize: 4,
            viewportMargin: Infinity
        });
        
        // Add output container for code cells
        if (type === 'code') {
            const cellOutput = document.createElement('div');
            cellOutput.className = 'cell-output';
            cellOutput.style.display = 'none';
            cell.appendChild(cellOutput);
            cell.outputElement = cellOutput;
        }
        
        // Store editor instance in cell
        cell.editor = editor;
        cell.type = type;
        cell.id = cell.dataset.id;
        
        // Add event handler for selecting cell
        cell.addEventListener('click', () => {
            if (activeCell) {
                activeCell.classList.remove('active');
            }
            activeCell = cell;
            cell.classList.add('active');
            editor.focus();
        });
        
        // Add the cell to the notebook
        notebookContainer.appendChild(cell);
        
        // Add to cells array
        cells.push(cell);
        
        // Refresh the editor to ensure it renders correctly
        setTimeout(() => editor.refresh(), 10);
        
        return cell;
    }

    // Function to create a default cell on load
    function createDefaultCell() {
        const cell = createCell('code', '# ยินดีต้อนรับสู่สมุดบันทึก AI Notebook\n# เริ่มเขียนโค้ดของคุณที่นี่');
        cell.classList.add('active');
        activeCell = cell;
    }

    // Function to run cell
    function runCell(cell) {
        if (!cell || cell.type !== 'code') return;
        
        const code = cell.editor.getValue();
        const outputElement = cell.outputElement;
        
        if (!code.trim()) {
            outputElement.textContent = '';
            outputElement.style.display = 'none';
            return;
        }
        
        // Show "running" indicator
        outputElement.textContent = 'กำลังประมวลผล...';
        outputElement.style.display = 'block';
        
        // In a real implementation, this would send the code to the server
        // For now, we'll simulate execution
        simulateCodeExecution(code)
            .then(result => {
                outputElement.textContent = result.output;
                outputElement.style.display = 'block';
                
                // If there's visualization data, display it
                if (result.visualization) {
                    appendVisualization(outputElement, result.visualization);
                }
            })
            .catch(error => {
                outputElement.textContent = `Error: ${error.message}`;
                outputElement.style.display = 'block';
            });
    }

    // Function to simulate code execution
    function simulateCodeExecution(code) {
        return new Promise((resolve, reject) => {
            setTimeout(() => {
                try {
                    let output = '';
                    let visualization = null;
                    
                    // Simulate different outputs based on code content
                    if (code.includes('print(')) {
                        // Extract content from print statements
                        const printMatches = code.match(/print\((.*?)\)/g);
                        if (printMatches) {
                            output = printMatches.map(match => {
                                const content = match.substring(6, match.length - 1).trim();
                                // Remove quotes for strings
                                return content.startsWith('"') && content.endsWith('"') ? 
                                    content.substring(1, content.length - 1) : 
                                    content;
                            }).join('\n');
                        }
                    } else if (code.includes('import matplotlib') || code.includes('plt.')) {
                        output = 'กราฟถูกสร้างเรียบร้อยแล้ว';
                        visualization = {
                            type: 'plot',
                            data: 'placeholder'  // In a real implementation, this would be plot data
                        };
                    } else if (code.includes('import numpy') || code.includes('np.')) {
                        const randomData = Array.from({length: 5}, () => 
                            Array.from({length: 5}, () => Math.floor(Math.random() * 100)));
                        output = randomData.map(row => row.join('\t')).join('\n');
                    } else if (code.includes('import pandas') || code.includes('pd.')) {
                        output = `   Column1  Column2  Column3
0       10       20       30
1       40       50       60
2       70       80       90`;
                    } else {
                        output = 'โค้ดทำงานเสร็จสิ้น โดยไม่มีเอาต์พุต';
                    }
                    
                    resolve({ output, visualization });
                } catch (error) {
                    reject(error);
                }
            }, 500);  // Simulate processing delay
        });
    }

    // Function to append visualization to output
    function appendVisualization(outputElement, visualData) {
        if (visualData.type === 'plot') {
            const plotDiv = document.createElement('div');
            plotDiv.className = 'visualization-plot';
            plotDiv.style.height = '300px';
            plotDiv.style.width = '100%';
            plotDiv.style.backgroundColor = '#f5f5f5';
            plotDiv.style.border = '1px solid #ddd';
            plotDiv.style.borderRadius = '4px';
            plotDiv.style.marginTop = '10px';
            plotDiv.style.display = 'flex';
            plotDiv.style.alignItems = 'center';
            plotDiv.style.justifyContent = 'center';
            
            // Placeholder content (in a real app, this would be a chart)
            plotDiv.innerHTML = '<div style="text-align:center;"><i class="fas fa-chart-line" style="font-size:48px;color:#4285f4;margin-bottom:8px;"></i><div>Matplotlib Plot Visualization</div></div>';
            
            outputElement.appendChild(plotDiv);
        }
    }

    // Function to delete cell
    function deleteCell(cell) {
        // Confirm deletion
        if (!confirm('คุณแน่ใจหรือไม่ว่าต้องการลบเซลล์นี้?')) {
            return;
        }
        
        const cellIndex = cells.indexOf(cell);
        if (cellIndex !== -1) {
            // Remove from cells array
            cells.splice(cellIndex, 1);
            
            // Remove from DOM
            cell.remove();
            
            // Update active cell if needed
            if (activeCell === cell) {
                activeCell = null;
                
                // Set next cell as active, or previous if no next
                if (cells.length > 0) {
                    const newActiveIndex = Math.min(cellIndex, cells.length - 1);
                    activeCell = cells[newActiveIndex];
                    activeCell.classList.add('active');
                    activeCell.editor.focus();
                }
            }
            
            showNotification('ลบเซลล์เรียบร้อยแล้ว', 'info');
        }
    }

    // Function to run all cells
    function runAllCells() {
        if (cells.length === 0) return;
        
        // Show notification
        showNotification('กำลังเรียกใช้ทุกเซลล์...', 'info');
        
        // Run cells sequentially
        let index = 0;
        function runNextCell() {
            if (index < cells.length) {
                const cell = cells[index++];
                if (cell.type === 'code') {
                    runCell(cell);
                    // Add delay between cells to simulate execution order
                    setTimeout(runNextCell, 700);
                } else {
                    runNextCell();
                }
            } else {
                showNotification('เรียกใช้ทุกเซลล์เรียบร้อยแล้ว', 'success');
            }
        }
        
        runNextCell();
    }

    // === UI Helpers ===

    // Function to show modal
    function showModal(modal) {
        modal.style.display = 'flex';
    }

    // Function to hide modal
    function hideModal(modal) {
        modal.style.display = 'none';
    }

    // Function to show notification
    function showNotification(message, type = 'info') {
        notificationMessage.textContent = message;
        
        // Apply type-specific styling
        notification.className = 'notification';
        notification.classList.add(`notification-${type}`);
        
        // Show notification
        notification.style.display = 'flex';
        
        // Auto-hide after a delay
        setTimeout(() => {
            hideNotification();
        }, 3000);
    }

    // Function to hide notification
    function hideNotification() {
        notification.style.display = 'none';
    }

    // Function to initialize menu interaction
    function initializeMenu() {
        // File menu actions
        document.getElementById('renameNotebook').addEventListener('click', renameNotebook);
        document.getElementById('saveNotebook').addEventListener('click', saveNotebook);
        document.getElementById('openNotebook').addEventListener('click', loadNotebook);
        
        // Download menu actions
        document.getElementById('downloadNotebook').addEventListener('click', () => downloadNotebook('ipynb'));
        document.getElementById('downloadPython').addEventListener('click', () => downloadNotebook('py'));
        document.getElementById('downloadHTML').addEventListener('click', () => downloadNotebook('html'));
        
        // Edit menu actions
        document.getElementById('clearAllOutputs').addEventListener('click', () => {
            cells.forEach(cell => {
                if (cell.type === 'code' && cell.outputElement) {
                    cell.outputElement.textContent = '';
                    cell.outputElement.style.display = 'none';
                }
            });
            showNotification('ล้างเอาต์พุตทั้งหมดเรียบร้อยแล้ว', 'info');
        });
        
        // View menu actions
        document.getElementById('toggleOutput').addEventListener('click', () => {
            const allHidden = cells.every(cell => 
                cell.type !== 'code' || !cell.outputElement || cell.outputElement.style.display === 'none');
                
            cells.forEach(cell => {
                if (cell.type === 'code' && cell.outputElement) {
                    cell.outputElement.style.display = allHidden ? 'block' : 'none';
                }
            });
            
            showNotification(allHidden ? 'แสดงเอาต์พุตทั้งหมด' : 'ซ่อนเอาต์พุตทั้งหมด', 'info');
        });
        
        // Runtime menu actions
        document.getElementById('runAllCells').addEventListener('click', runAllCells);
        document.getElementById('runFocusedCell').addEventListener('click', () => {
            if (activeCell) {
                runCell(activeCell);
            }
        });
        
        // Help menu actions
        document.getElementById('keyboard-shortcuts')?.addEventListener('click', () => {
            showModal(keyboardShortcutsModal);
        });
    }

    // Function to initialize side panel tabs
    function initializePanels() {
        panelTabs.forEach(tab => {
            tab.addEventListener('click', () => {
                // Update active state
                panelTabs.forEach(t => t.classList.remove('active'));
                tab.classList.add('active');
                
                // Show corresponding panel content
                const panelId = tab.getAttribute('data-panel');
                document.querySelectorAll('.panel-section').forEach(panel => {
                    panel.classList.remove('active');
                });
                document.getElementById(`${panelId}-panel`).classList.add('active');
                
                console.log(`Switching to view: ${panelId}`);
            });
        });
    }

    // === Event Listeners ===

    // Rename notebook event listeners
    renameButton.addEventListener('click', renameNotebook);
    notebookTitle.addEventListener('dblclick', renameNotebook);
    confirmRenameButton.addEventListener('click', confirmRename);
    cancelRenameButton.addEventListener('click', () => hideModal(renameModal));
    
    // Close modal buttons
    closeModalButtons.forEach(button => {
        button.addEventListener('click', () => {
            const modal = button.closest('.modal');
            if (modal) {
                hideModal(modal);
            }
        });
    });
    
    // Add cell buttons
    addCodeCellButton.addEventListener('click', () => {
        const index = activeCell ? cells.indexOf(activeCell) : cells.length - 1;
        const position = index !== -1 ? index + 1 : cells.length;
        
        // Create new cell and insert at position
        const newCell = createCell('code');
        
        // Move DOM element to the correct position
        if (position < cells.length - 1) {
            notebookContainer.insertBefore(newCell, cells[position + 1]);
            
            // Reorder cells array
            cells.pop(); // Remove from end (where it was added by createCell)
            cells.splice(position, 0, newCell);
        }
        
        // Set as active cell
        if (activeCell) {
            activeCell.classList.remove('active');
        }
        activeCell = newCell;
        newCell.classList.add('active');
        newCell.editor.focus();
    });
    
    addTextCellButton.addEventListener('click', () => {
        const index = activeCell ? cells.indexOf(activeCell) : cells.length - 1;
        const position = index !== -1 ? index + 1 : cells.length;
        
        // Create new cell and insert at position
        const newCell = createCell('markdown');
        
        // Move DOM element to the correct position
        if (position < cells.length - 1) {
            notebookContainer.insertBefore(newCell, cells[position + 1]);
            
            // Reorder cells array
            cells.pop(); // Remove from end (where it was added by createCell)
            cells.splice(position, 0, newCell);
        }
        
        // Set as active cell
        if (activeCell) {
            activeCell.classList.remove('active');
        }
        activeCell = newCell;
        newCell.classList.add('active');
        newCell.editor.focus();
    });
    
    // Run buttons
    runCellButton.addEventListener('click', () => {
        if (activeCell) {
            runCell(activeCell);
        } else {
            showNotification('ไม่มีเซลล์ที่เลือก', 'warning');
        }
    });
    
    runAllButton.addEventListener('click', runAllCells);
    
    // Save button
    saveNotebookButton.addEventListener('click', saveNotebook);
    
    // Close notification button
    closeNotificationButton.addEventListener('click', hideNotification);
    
    // Window click handler for modals
    window.addEventListener('click', (event) => {
        if (event.target.classList.contains('modal')) {
            hideModal(event.target);
        }
    });
    
    // Keyboard shortcuts
    document.addEventListener('keydown', (event) => {
        // Check if ctrl or cmd key is pressed
        const ctrlOrCmd = event.ctrlKey || event.metaKey;
        
        // Ctrl+S for save
        if (ctrlOrCmd && event.key === 's') {
            event.preventDefault();
            saveNotebook();
        }
        
        // Ctrl+Enter to run current cell
        if (ctrlOrCmd && event.key === 'Enter') {
            event.preventDefault();
            if (activeCell) {
                runCell(activeCell);
            }
        }
        
        // Shift+Enter to run cell and create new one
        if (event.shiftKey && event.key === 'Enter') {
            event.preventDefault();
            if (activeCell) {
                runCell(activeCell);
                
                // Create new cell
                const index = cells.indexOf(activeCell);
                const position = index !== -1 ? index + 1 : cells.length;
                
                // Create new cell and insert at position
                const newCell = createCell('code');
                
                // Move DOM element to the correct position
                if (position < cells.length - 1) {
                    notebookContainer.insertBefore(newCell, cells[position + 1]);
                    
                    // Reorder cells array
                    cells.pop(); // Remove from end (where it was added by createCell)
                    cells.splice(position, 0, newCell);
                }
                
                // Set as active cell
                activeCell.classList.remove('active');
                activeCell = newCell;
                newCell.classList.add('active');
                newCell.editor.focus();
            }
        }
    });
});