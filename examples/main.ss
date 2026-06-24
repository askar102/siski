func main() -> int {
    let var_example: int = 10;

    label loop:
        if (var_example >= 5) {
            goto stop;
        }
        var_example = var_example + 1;
        goto loop;

    label stop:
        return var_example;
}