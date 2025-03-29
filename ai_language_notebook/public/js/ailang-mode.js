// Custom CodeMirror mode for AI Language

(function(mod) {
    if (typeof exports == "object" && typeof module == "object") // CommonJS
        mod(require("../../lib/codemirror"));
    else if (typeof define == "function" && define.amd) // AMD
        define(["../../lib/codemirror"], mod);
    else // Plain browser env
        mod(CodeMirror);
})(function(CodeMirror) {
    "use strict";

    CodeMirror.defineMode("ailang", function() {
        // Keywords for AI Language
        const keywords = [
            "start", "create", "load", "clean", "split", "visualize", "plot",
            "train", "evaluate", "show", "save", "add", "layer", "compile",
            "predict", "using", "test", "configure", "set", "compare"
        ];

        // AI model types and special terms
        const modelTypes = [
            "ML", "DL", "RL", "LinearRegression", "LogisticRegression", 
            "RandomForest", "SVM", "KNN", "DecisionTree", "GradientBoosting", 
            "XGBoost", "CNN", "RNN", "LSTM", "GRU", "Transformer", "DQN", 
            "DDQN", "A2C", "PPO", "TRPO", "SAC"
        ];

        // Layer types for neural networks
        const layerTypes = [
            "convolutional", "max_pooling", "flatten", "dense", "dropout", 
            "batch_norm", "lstm", "gru", "embedding", "attention"
        ];

        // Parameters commonly used
        const parameters = [
            "with", "for", "into", "on", "to", "ratio", "type", "dataset", 
            "data", "model", "agent", "environment", "epochs", "batch_size", 
            "learning_rate", "filters", "kernel_size", "activation", "optimizer", 
            "loss", "metrics", "patience", "validation_data", "episodes", "gamma", 
            "epsilon", "drop_na", "fill_mean", "handle_outliers"
        ];

        return {
            startState: function() {
                return {
                    inString: false,
                    stringType: null,
                    inComment: false
                };
            },

            token: function(stream, state) {
                // Handle comments
                if (stream.match("#")) {
                    stream.skipToEnd();
                    return "comment";
                }

                // Handle strings
                if (state.inString) {
                    if (stream.eat('\\')) {
                        stream.next();  // Skip the escaped character
                        return "string";
                    }
                    if (stream.eat(state.stringType)) {
                        state.inString = false;
                        state.stringType = null;
                    } else {
                        stream.next();
                    }
                    return "string";
                }

                if (stream.eat('"') || stream.eat("'")) {
                    state.inString = true;
                    state.stringType = stream.backUp(1) && stream.next();
                    return "string";
                }

                // Handle numbers
                if (stream.match(/^-?\d+(\.\d+)?/)) {
                    return "number";
                }

                // Handle keywords, model types, etc.
                if (stream.match(/^\w+/)) {
                    const word = stream.current();
                    
                    if (keywords.includes(word)) {
                        return "keyword";
                    }
                    
                    if (modelTypes.includes(word)) {
                        return "def";  // Model types
                    }
                    
                    if (layerTypes.includes(word)) {
                        return "variable-3";  // Layer types
                    }
                    
                    if (parameters.includes(word)) {
                        return "variable-2";  // Parameters
                    }
                }

                // Skip other characters
                stream.next();
                return null;
            }
        };
    });

    CodeMirror.defineMIME("text/x-ailang", "ailang");
});