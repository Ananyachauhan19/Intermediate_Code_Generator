const express = require('express');
const cors = require('cors');
const fs = require('fs').promises;
const path = require('path');
const { exec } = require('child_process');
const util = require('util');

const execPromise = util.promisify(exec);

// Initialize Express app
const app = express();
const PORT = 3000;

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static(path.join(__dirname, '../frontend')));

// Paths configuration
const PROJECT_ROOT = path.join(__dirname, '..');
const INPUT_FILE = path.join(PROJECT_ROOT, 'input.txt');
const OUTPUTS_DIR = path.join(PROJECT_ROOT, 'outputs');
const COMPILER_EXECUTABLE = path.join(PROJECT_ROOT, 'icg.exe');

// Ensure outputs directory exists
async function ensureOutputsDir() {
    try {
        await fs.mkdir(OUTPUTS_DIR, { recursive: true });
    } catch (error) {
        console.error('Error creating outputs directory:', error);
    }
}

// Health check endpoint
app.get('/api/health', (req, res) => {
    res.json({ 
        status: 'ok', 
        message: 'Server is running',
        timestamp: new Date().toISOString()
    });
});

// Main compilation endpoint
app.post('/api/compile', async (req, res) => {
    try {
        const { code } = req.body;

        if (!code || code.trim() === '') {
            return res.status(400).json({
                success: false,
                error: 'No code provided'
            });
        }

        console.log('Received code for compilation:');
        console.log(code);

        // Step 1: Save input to file
        await fs.writeFile(INPUT_FILE, code, 'utf8');
        console.log('Input saved to:', INPUT_FILE);

        // Step 2: Run the compiler
        let compilerOutput;
        try {
            // Check if executable exists
            const fs = require('fs');
            if (!fs.existsSync(COMPILER_EXECUTABLE)) {
                console.log('Executable not found. Please build the compiler first using build.bat');
                return res.status(500).json({
                    success: false,
                    error: 'Compiler executable not found. Please run build.bat first to compile the C++ code.'
                });
            }

            // Run the compiled executable with input file piped to stdin
            console.log('Running compiler:', COMPILER_EXECUTABLE);
            const { stdout, stderr } = await execPromise(`type "${INPUT_FILE}" | "${COMPILER_EXECUTABLE}"`, {
                cwd: PROJECT_ROOT,
                timeout: 10000, // 10 second timeout
                shell: 'cmd.exe'
            });
            
            compilerOutput = stdout;
            if (stderr) {
                console.error('Compiler stderr:', stderr);
            }
            console.log('Compiler executed successfully');
            
        } catch (execError) {
            console.error('Execution error:', execError);
            return res.status(500).json({
                success: false,
                error: 'Failed to execute compiler. Error: ' + execError.message + '. Make sure to run build.bat first.'
            });
        }

        // Step 3: Read output files
        const outputs = await readOutputFiles();

        // Step 4: Return results
        res.json({
            success: true,
            outputs: outputs,
            compilerOutput: compilerOutput
        });

    } catch (error) {
        console.error('Compilation error:', error);
        res.status(500).json({
            success: false,
            error: error.message || 'An error occurred during compilation'
        });
    }
});

// Read all output files
async function readOutputFiles() {
    const outputs = {
        tac: '',
        quadruples: '',
        symbolTable: '',
        optimized: ''
    };

    const files = [
        { key: 'tac', file: 'tac.txt' },
        { key: 'quadruples', file: 'quadruples.txt' },
        { key: 'symbolTable', file: 'symbol_table.txt' },
        { key: 'optimized', file: 'optimized_tac.txt' }
    ];

    for (const { key, file } of files) {
        try {
            const filePath = path.join(OUTPUTS_DIR, file);
            const content = await fs.readFile(filePath, 'utf8');
            outputs[key] = content;
            console.log(`Read ${file}: ${content.length} bytes`);
        } catch (error) {
            console.warn(`Could not read ${file}:`, error.message);
            outputs[key] = `Output file not generated: ${file}`;
        }
    }

    return outputs;
}

// Get individual output file
app.get('/api/output/:type', async (req, res) => {
    const { type } = req.params;
    
    const fileMap = {
        'tac': 'tac.txt',
        'quadruples': 'quadruples.txt',
        'symbol': 'symbol_table.txt',
        'optimized': 'optimized_tac.txt'
    };

    const filename = fileMap[type];
    if (!filename) {
        return res.status(400).json({ error: 'Invalid output type' });
    }

    try {
        const filePath = path.join(OUTPUTS_DIR, filename);
        const content = await fs.readFile(filePath, 'utf8');
        res.json({ success: true, content });
    } catch (error) {
        res.status(404).json({ 
            success: false, 
            error: 'Output file not found. Please compile first.' 
        });
    }
});

// Serve frontend
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '../frontend/index.html'));
});

// Start server
async function startServer() {
    await ensureOutputsDir();
    
    app.listen(PORT, () => {
        console.log('🚀 Intermediate Code Generator Server');
        console.log(`Server running at: http://localhost:${PORT}`);
    });
}

// Handle graceful shutdown
process.on('SIGINT', () => {
    console.log('\n\n👋 Shutting down server...');
    process.exit(0);
});

// Start the server
startServer().catch(error => {
    console.error('Failed to start server:', error);
    process.exit(1);
});
