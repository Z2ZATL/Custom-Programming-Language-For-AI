/**
 * AI Language Notebook Server
 * Express server for the AI Language Notebook frontend
 */

const express = require('express');
const path = require('path');
const fs = require('fs');
const { exec } = require('child_process');
const multer = require('multer');
const { Pool } = require('pg');

// Database Connection
const pool = new Pool({
    connectionString: process.env.DATABASE_URL
});

// Create uploads directory if it doesn't exist
const uploadsDir = path.join(__dirname, 'uploads');
if (!fs.existsSync(uploadsDir)) {
    fs.mkdirSync(uploadsDir, { recursive: true });
}

// Configure multer for file uploads
const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        cb(null, uploadsDir);
    },
    filename: function (req, file, cb) {
        const uniqueSuffix = Date.now() + '-' + Math.round(Math.random() * 1E9);
        cb(null, uniqueSuffix + '-' + file.originalname);
    }
});
const upload = multer({ storage: storage });

const app = express();
const PORT = 5000;

// Middleware
app.use(express.json());
app.use((req, res, next) => {
    res.setHeader(
        'Content-Security-Policy',
        "script-src 'self' 'unsafe-eval' 'unsafe-inline'"
    );
    next();
});

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
    
    // Try first in local examples directory, then in ai_language directory
    let exampleFile = path.join(
        __dirname,
        'examples',
        `${type.toLowerCase()}_example.ai`
    );
    
    // If not found in local directory, try in ai_language directory
    if (!fs.existsSync(exampleFile)) {
        exampleFile = path.join(
            __dirname, 
            '..', 
            'ai_language', 
            'examples', 
            `${type.toLowerCase()}_example.ai`
        );
    }
    
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

// Upload a dataset file
app.post('/api/datasets/upload', upload.single('file'), async (req, res) => {
    try {
        if (!req.file) {
            return res.status(400).json({ error: 'No file uploaded' });
        }

        // Get file info
        const { originalname, mimetype, path: filePath, size } = req.file;
        const fileType = mimetype.split('/')[1] || 'unknown';
        const { name, description } = req.body;

        // Insert file record into database
        const result = await pool.query(
            'INSERT INTO datasets (name, description, file_path, file_type, file_size) VALUES ($1, $2, $3, $4, $5) RETURNING *',
            [name || originalname, description || '', filePath, fileType, size]
        );

        return res.status(201).json({
            message: 'File uploaded successfully',
            dataset: result.rows[0]
        });
    } catch (error) {
        console.error('Error uploading file:', error);
        return res.status(500).json({ error: `Upload failed: ${error.message}` });
    }
});

// Get all datasets
app.get('/api/datasets', async (req, res) => {
    try {
        const result = await pool.query('SELECT * FROM datasets ORDER BY created_at DESC');
        return res.json(result.rows);
    } catch (error) {
        console.error('Error fetching datasets:', error);
        return res.status(500).json({ error: `Database error: ${error.message}` });
    }
});

// Create a model
app.post('/api/models', async (req, res) => {
    try {
        const { name, model_type, description, parameters } = req.body;

        if (!name || !model_type) {
            return res.status(400).json({ error: 'Name and model_type are required' });
        }

        const result = await pool.query(
            'INSERT INTO models (name, model_type, description, parameters) VALUES ($1, $2, $3, $4) RETURNING *',
            [name, model_type, description || '', parameters || {}]
        );

        return res.status(201).json({
            message: 'Model created successfully',
            model: result.rows[0]
        });
    } catch (error) {
        console.error('Error creating model:', error);
        return res.status(500).json({ error: `Database error: ${error.message}` });
    }
});

// Get all models
app.get('/api/models', async (req, res) => {
    try {
        const result = await pool.query('SELECT * FROM models ORDER BY created_at DESC');
        return res.json(result.rows);
    } catch (error) {
        console.error('Error fetching models:', error);
        return res.status(500).json({ error: `Database error: ${error.message}` });
    }
});

// Record training results
app.post('/api/training-results', async (req, res) => {
    try {
        const { model_id, dataset_id, metrics, duration } = req.body;

        if (!model_id || !dataset_id) {
            return res.status(400).json({ error: 'model_id and dataset_id are required' });
        }

        const result = await pool.query(
            'INSERT INTO training_results (model_id, dataset_id, metrics, duration) VALUES ($1, $2, $3, $4) RETURNING *',
            [model_id, dataset_id, metrics || {}, duration || 0]
        );

        return res.status(201).json({
            message: 'Training results recorded successfully',
            result: result.rows[0]
        });
    } catch (error) {
        console.error('Error recording training results:', error);
        return res.status(500).json({ error: `Database error: ${error.message}` });
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