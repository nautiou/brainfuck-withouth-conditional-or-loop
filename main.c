#include <stdlib.h>
#include <unistd.h>

typedef unsigned int uint;

struct bf {
    unsigned char memory[0xff];
    unsigned char p;
    unsigned char *prog;
};

unsigned char const C[255] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,3,8,4,7,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,5,0,6,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

void inf(struct bf * bf);
void sup(struct bf * bf);
void add(struct bf * bf);
void sub(struct bf * bf);
void ent(struct bf * bf);
void ret(struct bf * bf);
void out(struct bf * bf);
void inp(struct bf * bf);
void(*Command[])(struct bf *) = {ret, inf, sup, add, sub, ent, ret, out, inp};

void ret(struct bf *bf) {}
void run(struct bf * bf) {
    Command[*bf->prog](bf);
}
void inf(struct bf *bf) {
    bf->p--;
    Command[*++bf->prog](bf);
}
void sup(struct bf *bf) {
    bf->p++;
    Command[*++bf->prog](bf);
}
void add(struct bf *bf) {
    bf->memory[bf->p]++;
    Command[*++bf->prog](bf);
}
void sub(struct bf *bf) {
    bf->memory[bf->p]--;
    Command[*++bf->prog](bf);
}
void pass(struct bf * bf) {
    static void(*f[2])(struct bf*) = {pass, ret};
    f[*++bf->prog == 6](bf);
    Command[*++bf->prog](bf);
}
void ent(struct bf *bf) {
    static void(*f[2])(struct bf*) = {run, pass};
    unsigned char * ptr = bf->prog++;
    f[!bf->memory[bf->p]](bf);
    unsigned char *tmp[2] = {ptr - 1, bf->prog};
    bf->prog = tmp[!bf->memory[bf->p]];
    Command[*++bf->prog](bf);
}
void out(struct bf *bf) {
    write(1, bf->memory + bf->p, 1);
    Command[*++bf->prog](bf);
}
void inp(struct bf *bf) {
    read(0, bf->memory + bf->p, 1);
    Command[*++bf->prog](bf);
}

void reset_end(char *s, uint len) {}
void reset(char * s, uint len) {
    static void(*f[2])(char*, uint) = {reset, reset_end};
    f[!len](s + 1, len - 1);
    *s = 0;
}

int proglen_end(char * str) {
    return 0;
}
int proglen(char * str)
{
    static int(*f[2])(char*) = {proglen, proglen_end};
    return !!C[*str] + f[!*str](str+1);
}

void parse_input_end(char *input, char *result, uint len){
    *result = 0;
}
void parse_input(char *input, char *result, uint len)
{
    static void (*f[2])(char*, char*, uint) = {parse_input, parse_input_end};
    char value = C[*input];
    char next = !!value;
    *result = value;
    f[len == 0](input + 1, result + next, len - next);
}

void no_init(char * str, uint len) {}
void init(char * str, uint len) {
    struct bf bf;
    char prog[len + 1];
    parse_input(str, prog, len-1);
    reset(bf.memory, 0xff);
    bf.prog = prog;
    bf.p = 0;
    Command[*prog](&bf);
}

void no_start(char * str) {}
void start(char * str) {
    static void(*f[2])(char *, uint) = {init, no_init};
    uint length = proglen(str);
    f[!length](str, length);
}

int main(int ac, char *av[])
{
    static void(*f[2])(char*) = {no_start, start};
    f[ac == 2](av[1]);
    return 0;
}
