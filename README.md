# Data Connection Protocol

Resolution proposal for the first project of the [Computer Networks](https://sigarra.up.pt/feup/en/UCURR_GERAL.FICHA_UC_VIEW?pv_ocorrencia_id=333125) course unit @ FEUP

## Binaries and Releases

[Head here to download the binary releases](https://github.com/ctrlMarcio/feup-rcom-proj1/releases).

## Usage

### Sender

```bash
sender [-v] <filename>
```

> where
>
> - `-v` is the flag that indicates the usage of virtual serial ports
> - `--virtual` is an alias for `-v`
> - \<filename\> is the name of the file to transfer
>   - the file name accepts relative paths, take into account that the receiver will also use that relative path

### Receiver

```bash
receiver [-v]
```

> where
>
> - `-v` is the flag that indicates the usage of virtual serial ports
> - `--virtual` is an alias for `-v`

## Errors

Apart from printing an indicative message to `stderr` in case of an error, the program will also return a value from the following list:

| Return code                   | Error |
| ----------------------------- | ----- |
| Sender arguments error        | -1    |
| Receiver arguments error      | -2    |
| Port configuration error      | -3    |
| Establishing connection error | -4    |
| Closing connection error      | -5    |
| Lost frame                    | -6    |
| Lost start packet             | -7    |
| Lost end packet               | -8    |
| Lost data packet error        | -9    |
| Invalid control packet type   | -10   |
| Opening/Closing file error    | -11   |
| Already existing file error   | -12   |
| Repeated frame error          | -13   |

## Authors

Made with ❤ by T3G09:

- Flávia Carvalhido, up201806857
- Márcio Duarte, up201909936

## Documentation

The documentation of this project was made and compiled using [Doxygen](https://www.doxygen.nl/index.html), and can be accessed [right here](https://ctrlmarcio.github.io/feup-rcom-proj1/files.html).

## License

[MIT](https://opensource.org/licenses/MIT)