// Configuration
const API_BASE_URL = 'http://localhost:3000/api';

// DOM Elements
const codeInput = document.getElementById('codeInput');
const compileBtn = document.getElementById('compileBtn');
const clearBtn = document.getElementById('clearBtn');
const statusMessage = document.getElementById('statusMessage');
const serverStatus = document.getElementById('serverStatus');

// Output elements
const tacOutput = document.getElementById('tacOutput');
const quadruplesOutput = document.getElementById('quadruplesOutput');
const symbolOutput = document.getElementById('symbolOutput');
const optimizedOutput = document.getElementById('optimizedOutput');

// Tab functionality
const tabButtons = document.querySelectorAll('.tab-btn');
const tabContents = document.querySelectorAll('.tab-content');

// Initialize the application
function init() {
    // Load saved input from localStorage if available
    const savedInput = localStorage.getItem('compilerInput');
    if (savedInput) {
        codeInput.value = savedInput;
    }

    // Set up event listeners
    setupEventListeners();
    
    // Check server status
    checkServerStatus();
}

// Set up all event listeners
function setupEventListeners() {
    // Button event listeners
    compileBtn.addEventListener('click', compileCode);
    clearBtn.addEventListener('click', clearAll);

    // Tab switching
    tabButtons.forEach(button => {
        button.addEventListener('click', () => switchTab(button.dataset.tab));
    });

    // Auto-save input to localStorage
    codeInput.addEventListener('input', () => {
        localStorage.setItem('compilerInput', codeInput.value);
    });

    // Keyboard shortcuts
    document.addEventListener('keydown', (e) => {
        // Ctrl+Enter to compile
        if (e.ctrlKey && e.key === 'Enter') {
            e.preventDefault();
            compileCode();
        }
    });
}

// Check if server is running
async function checkServerStatus() {
    try {
        const response = await fetch(`${API_BASE_URL}/health`, {
            method: 'GET',
            mode: 'cors'
        });
        
        if (response.ok) {
            serverStatus.textContent = 'Online';
            serverStatus.className = 'status-indicator online';
        } else {
            throw new Error('Server not responding');
        }
    } catch (error) {
        serverStatus.textContent = 'Offline';
        serverStatus.className = 'status-indicator offline';
        showStatus('⚠️ Backend server is not running. Please start the server first.', 'warning');
    }
}

// Switch between tabs
function switchTab(tabName) {
    // Remove active class from all buttons and contents
    tabButtons.forEach(btn => btn.classList.remove('active'));
    tabContents.forEach(content => content.classList.remove('active'));

    // Add active class to selected tab
    const selectedButton = document.querySelector(`[data-tab="${tabName}"]`);
    const selectedContent = document.getElementById(tabName);

    if (selectedButton && selectedContent) {
        selectedButton.classList.add('active');
        selectedContent.classList.add('active');
    }
}

// Show status message
function showStatus(message, type = 'info') {
    statusMessage.textContent = message;
    statusMessage.className = `status-message show ${type}`;
    
    // Auto-hide after 5 seconds for success messages
    if (type === 'success') {
        setTimeout(() => {
            statusMessage.classList.remove('show');
        }, 5000);
    }
}

// Hide status message
function hideStatus() {
    statusMessage.classList.remove('show');
}

// Compile code - main function
async function compileCode() {
    const code = codeInput.value.trim();
    
    if (!code) {
        showStatus('❌ Please enter some code first.', 'error');
        return;
    }

    // Disable button and show loading state
    compileBtn.disabled = true;
    compileBtn.classList.add('loading');
    showStatus('⏳ Compiling your code...', 'info');

    try {
        // Send code to backend
        const response = await fetch(`${API_BASE_URL}/compile`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ code: code })
        });

        const data = await response.json();

        if (response.ok && data.success) {
            // Display all outputs
            displayOutputs(data.outputs);
            showStatus('✅ Compilation successful! Results displayed below.', 'success');
        } else {
            throw new Error(data.error || 'Compilation failed');
        }
    } catch (error) {
        showStatus(`❌ Error: ${error.message}`, 'error');
        console.error('Compilation error:', error);
    } finally {
        // Re-enable button and remove loading state
        compileBtn.disabled = false;
        compileBtn.classList.remove('loading');
    }
}

// Display all outputs in their respective tabs
function displayOutputs(outputs) {
    // Three Address Code
    if (outputs.tac) {
        tacOutput.textContent = outputs.tac;
        tacOutput.classList.remove('empty');
    } else {
        tacOutput.textContent = 'No TAC output generated.';
        tacOutput.classList.add('empty');
    }

    // Quadruples
    if (outputs.quadruples) {
        quadruplesOutput.textContent = outputs.quadruples;
        quadruplesOutput.classList.remove('empty');
    } else {
        quadruplesOutput.textContent = 'No quadruples output generated.';
        quadruplesOutput.classList.add('empty');
    }

    // Symbol Table
    if (outputs.symbolTable) {
        symbolOutput.textContent = outputs.symbolTable;
        symbolOutput.classList.remove('empty');
    } else {
        symbolOutput.textContent = 'No symbol table output generated.';
        symbolOutput.classList.add('empty');
    }

    // Optimized Code
    if (outputs.optimized) {
        optimizedOutput.textContent = outputs.optimized;
        optimizedOutput.classList.remove('empty');
    } else {
        optimizedOutput.textContent = 'No optimized output generated.';
        optimizedOutput.classList.add('empty');
    }
}

// Clear all inputs and outputs
function clearAll() {
    if (confirm('Are you sure you want to clear all inputs and outputs?')) {
        // Clear input
        codeInput.value = '';
        localStorage.removeItem('compilerInput');

        // Clear all outputs
        const emptyMessage = 'No output yet. Click "Compile & Generate" to process your code.';
        tacOutput.textContent = emptyMessage;
        quadruplesOutput.textContent = emptyMessage;
        symbolOutput.textContent = emptyMessage;
        optimizedOutput.textContent = emptyMessage;

        // Add empty class
        tacOutput.classList.add('empty');
        quadruplesOutput.classList.add('empty');
        symbolOutput.classList.add('empty');
        optimizedOutput.classList.add('empty');

        showStatus('✅ All content cleared.', 'success');
    }
}

// Initialize the application when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    init();
    
    // Show welcome message
    setTimeout(() => {
        showStatus('👋 Welcome! Enter your code and press Ctrl+Enter or click compile.', 'info');
    }, 1000);
});
