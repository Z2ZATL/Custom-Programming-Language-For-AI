
const express = require('express');
const { spawn } = require('child_process');
const app = express();

app.use(express.json());
app.use(express.static('.'));

app.post('/api/run', async (req, res) => {
    const { code } = req.body;
    
    try {
        const process = spawn('./ai_lang', ['--execute']);
        let output = '';
        
        process.stdin.write(code);
        process.stdin.end();
        
        process.stdout.on('data', (data) => {
            output += data.toString();
        });
        
        process.stderr.on('data', (data) => {
            output += data.toString();
        });
        
        process.on('close', (code) => {
            res.json({ output });
        });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }
});

app.listen(5000, '0.0.0.0', () => {
    console.log('Server running on port 5000');
});
