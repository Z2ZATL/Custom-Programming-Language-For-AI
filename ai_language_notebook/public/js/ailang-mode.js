/**
 * CodeMirror syntax highlighting mode for AI Language
 */

(function(mod) {
    if (typeof exports == "object" && typeof module == "object")
        mod(require("../../lib/codemirror"));
    else if (typeof define == "function" && define.amd)
        define(["../../lib/codemirror"], mod);
    else
        mod(CodeMirror);
})(function(CodeMirror) {
    "use strict";

    CodeMirror.defineMode("ailang", function() {
        function words(str) {
            var obj = {}, words = str.split(" ");
            for (var i = 0; i < words.length; ++i) obj[words[i]] = true;
            return obj;
        }

        // Keywords for the AI language
        var keywords = words(
            "start create model train evaluate visualize load clean split merge " +
            "with on for into using by when if else import export save show " + 
            "configure add from to where import export optimize tune analyze " +
            "predict deploy batch normalize"
        );

        // AI models and types
        var builtin = words(
            "ML DL RL LinearRegression LogisticRegression DecisionTree RandomForest " +
            "SVM KNN NaiveBayes GradientBoosting CNN RNN LSTM GAN GRU Transformer " +
            "BERT GPT DQN A3C PPO DDPG"
        );

        // Operators and symbols
        var atoms = words("true false null");

        // Types and special parameters
        var specials = words(
            "epochs learning_rate batch_size optimizer loss metrics activation " +
            "layers units kernel_size dropout ratio test_size random_state verbose " +
            "shuffle validation_split momentum weight_decay steps gamma epsilon " +
            "filters kernel_size stride padding embed_dim heads feed_forward"
        );

        var isOperatorChar = /[+\-*&%=<>!?|\/]/;

        function tokenBase(stream, state) {
            var ch = stream.next();
            
            // Handle comments
            if (ch === "#") {
                stream.skipToEnd();
                return "comment";
            }
            
            // Handle strings
            if (ch === '"' || ch === "'") {
                state.tokenize = tokenString(ch);
                return state.tokenize(stream, state);
            }
            
            // Handle numbers
            if (/\d/.test(ch)) {
                stream.eatWhile(/[\d\.]/);
                return "number";
            }
            
            // Handle operators
            if (isOperatorChar.test(ch)) {
                stream.eatWhile(isOperatorChar);
                return "operator";
            }
            
            // Handle identifiers and keywords
            if (/[a-zA-Z_]/.test(ch)) {
                stream.eatWhile(/[\w_]/);
                var word = stream.current();
                
                if (keywords.hasOwnProperty(word)) return "keyword";
                if (builtin.hasOwnProperty(word)) return "builtin";
                if (atoms.hasOwnProperty(word)) return "atom";
                if (specials.hasOwnProperty(word)) return "special";
                
                return "variable";
            }
            
            // Default style for other characters
            return null;
        }

        function tokenString(quote) {
            return function(stream, state) {
                var escaped = false, next, end = false;
                
                while ((next = stream.next()) != null) {
                    if (next === quote && !escaped) {
                        end = true;
                        break;
                    }
                    escaped = !escaped && next === "\\";
                }
                
                if (end || !escaped) state.tokenize = tokenBase;
                return "string";
            };
        }

        return {
            startState: function() {
                return {
                    tokenize: tokenBase,
                    context: null
                };
            },
            
            token: function(stream, state) {
                if (stream.eatSpace()) return null;
                return state.tokenize(stream, state);
            },
            
            lineComment: "#"
        };
    });

    CodeMirror.defineMIME("text/x-ailang", "ailang");
});