![](https://user-images.githubusercontent.com/33803413/125828823-8eb341c2-6ca4-4f17-865f-1bd4ea0cf9c5.png)

# Histogram

A histogram generator.

## Building

``` shell
cmake . -Bbuild
cmake --build build
```

## Usage

``` shell
histogram [options]
```

`histogram` reads integers from `stdin` until `EOF` and prints a histogram with the bar sizes corresponding to the input data.

### Options

Option | Meaning
--- | ---
`--char <char>` | Specify the ASCII character used for printing the histogram. This option has no effect when the `--pretty` option is given.
`--help` | Show help message and exit.
`--layout <layout>` | Specify the layout of the bars in the histogram. `<layout>` can be `horizontal` or `vertical`.
`--pretty` | Pretty-print the histogram using Unicode Box Drawing Characters. This option requires a terminal with UTF-8 encoding to work properly.

## License

[MIT](https://github.com/wadiim/histogram/blob/master/LICENSE)
