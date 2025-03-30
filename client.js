
class NotebookCell {
    constructor(container) {
        this.container = document.createElement('div');
        this.container.className = 'cell';
        
        const toolbar = document.createElement('div');
        toolbar.className = 'cell-toolbar';
        
        const runButton = document.createElement('button');
        runButton.textContent = 'Run';
        runButton.onclick = () => this.runCode();
        
        toolbar.appendChild(runButton);
        this.container.appendChild(toolbar);
        
        this.editor = CodeMirror(this.container, {
            mode: 'python',
            theme: 'monokai',
            lineNumbers: true,
            lineWrapping: true
        });
        
        this.output = document.createElement('div');
        this.output.className = 'cell-output';
        this.container.appendChild(this.output);
        
        container.appendChild(this.container);
    }
    
    async runCode() {
        const code = this.editor.getValue();
        try {
            const response = await fetch('/api/run', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ code })
            });
            const result = await response.json();
            this.output.textContent = result.output;
        } catch (error) {
            this.output.textContent = `Error: ${error.message}`;
        }
    }
}

document.addEventListener('DOMContentLoaded', () => {
    const cellsContainer = document.getElementById('cells');
    document.getElementById('addCell').onclick = () => {
        new NotebookCell(cellsContainer);
    };
    
    // Create initial cell
    new NotebookCell(cellsContainer);
});
