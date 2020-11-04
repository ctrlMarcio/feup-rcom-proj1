# feup-rcom-proj1

Resolution proposal for the first project of the Computer Networks course unit @ FEUP

Made with ❤ by T3G09:

- Flávia Carvalhido, up201806857
- Márcio Duarte, up201909936

## Usage

### Sender

```bash
sender [-v] <filename>
```

> where
>
> - `-v` is the flag that indicates the usage of virtual serial ports
> - `--virtual` is an alias for `-v`
> - <filename> is the name of the file to transfer

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

## License

[MIT](https://opensource.org/licenses/MIT)