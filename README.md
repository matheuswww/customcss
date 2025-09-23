# CustomCSS

CustomCSS is a small framework written in C that converts .customcss rules into a valid .css file. It allows you to define class-like shortcuts and even use dynamic values.

## How it works

Create rules in a .customcss file. Example:

text-green: { color: green; }
h: { height: [value]; }
w: { width: [value]; }

Everything inside {} is raw CSS. `[value]` is a placeholder for dynamic values.

You can use dynamic values in your HTML by writing the class with -[value]. For example:

&lt;div class="h-[100px] w-[50%] bg-blue text-green">Hello&lt;/div>

This will generate CSS like:

.h-100px { height: 100px; }
.w-50\% { width: 50%; }
.bg-blue { background-color: blue; }
.text-green { color: green; }

The program parses these rules and scans the HTML file, generating a final .css stylesheet with only the classes that are actually used.

## Features

- Define reusable CSS shortcuts in .customcss format.
- Support for static rules (e.g., bg-blue).
- Support for dynamic rules with `[value]` (e.g., h-[200px]).
- Generates a clean `.css` file ready to link in your HTML.

## How to build and run

1. Build the project using `make`:

2. Run the program with three arguments:

- Path to the `.customcss` file.
- Path to the HTML file to scan.
- Path where the generated `.css` file should be saved.