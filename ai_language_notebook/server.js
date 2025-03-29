/**
 * AI Language Notebook Server
 * Express server for the AI Language Notebook frontend
 */

const express = require('express');
const path = require('path');
const fs = require('fs');
const { exec } = require('child_process');

const app = express();
const PORT = 5000;

// Middleware
app.use(express.json());
app.use(express.static(path.join(__dirname, 'public')));

// APIs

// Execute AI Language code
app.post('/api/execute', (req, res) => {
    const { code } = req.body;
    
    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }
    
    // Create a temporary file
    const tempFile = path.join(__dirname, `temp_${Date.now()}.ai`);
    
    try {
        // Write code to temp file
        fs.writeFileSync(tempFile, code);
        
        // Execute code with ai_lang interpreter
        const aiLangPath = path.join(__dirname, '..', 'ai_language', 'ai_lang');
        
        exec(`${aiLangPath} ${tempFile}`, (error, stdout, stderr) => {
            // Clean up temp file
            try {
                fs.unlinkSync(tempFile);
            } catch (err) {
                console.error('Error removing temp file:', err);
            }
            
            if (error) {
                console.error(`Execution error: ${error.message}`);
                return res.status(500).json({ 
                    error: `Error executing code: ${error.message}`,
                    stderr: stderr
                });
            }
            
            if (stderr) {
                console.warn(`Stderr: ${stderr}`);
            }
            
            // Return the output
            return res.json({ 
                text: stdout,
                visuals: getVisualizationsFromCode(code, stdout)
            });
        });
    } catch (error) {
        console.error('Server error:', error);
        
        // Clean up temp file if it exists
        if (fs.existsSync(tempFile)) {
            try {
                fs.unlinkSync(tempFile);
            } catch (err) {
                console.error('Error removing temp file:', err);
            }
        }
        
        return res.status(500).json({ error: `Server error: ${error.message}` });
    }
});

// Get example code by type
app.get('/api/examples/:type', (req, res) => {
    const { type } = req.params;
    
    // Validate type
    if (!['ml', 'dl', 'rl'].includes(type.toLowerCase())) {
        return res.status(400).json({ error: 'Invalid example type' });
    }
    
    // Get example file path
    const exampleFile = path.join(
        __dirname, 
        '..', 
        'ai_language', 
        'examples', 
        `${type.toLowerCase()}_example.ai`
    );
    
    try {
        // Check if file exists
        if (!fs.existsSync(exampleFile)) {
            return res.status(404).json({ error: 'Example file not found' });
        }
        
        // Read file
        const content = fs.readFileSync(exampleFile, 'utf8');
        
        // Split into lines
        const lines = content.split('\n');
        
        return res.json({ lines });
    } catch (error) {
        console.error('Error loading example:', error);
        return res.status(500).json({ error: `Error loading example: ${error.message}` });
    }
});

// Function to extract visualizations based on code
function getVisualizationsFromCode(code, output) {
    // This is a simplified version that checks if the code contains visualization commands
    if (code.includes('visualize') || code.includes('plot')) {
        // If the code has visualization commands, provide a placeholder response
        // In a real implementation, you'd generate actual visualization data
        return {
            type: 'plot',
            data: null // In a real implementation, this would be the visualization data
        };
    }
    
    return null;
}

// Fallback route to serve index.html for any unmatched routes
app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Start the server
app.listen(PORT, () => {
    console.log(`AI Language Notebook server running at http://localhost:${PORT}`);
    console.log(`Open your browser and navigate to http://localhost:${PORT}`);
});