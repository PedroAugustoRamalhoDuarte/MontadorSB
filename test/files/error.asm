SECTION TEXT
INPUT OLD_DATA
LOAD OLD_DATA
L1: DIV DOIS
STORE NEW_DATA
MULT DOIS
STORE TMP_DATA
LOAD OLD_DATA
COPY OLD_DATA, OLD_DATA
COPY OLD_DATA OLD_DATA ; formato argumento errado
LABEL: LABEL: STOP ; mais de uma label;
9teste: STOP ; token inválido
LOAD 9OLD_DATA ; token inválido
LOAD OLD_DATA OLD_DATA OLD_DATA ; quantidade de argumentos invalida
INPUT
INPUT OLD_DATA OLD_DATA
copy simboloindefinido, simboloindefinido
INPUT simboloindefinido
SUB TMP_DATA
STORE TMP_DATA
OUTPUT TMP_DATA
COPY NEW_DATA, OLD_DATA
LOAD OLD_DATA
JMPP L1
STOP
SECTION DATA
DOIS: CONST OAS
EAE: CONST -1
EAEE: CONST 100
OLD_DATA: SPACE
NEW_DATA: SPACE
TMP_DATA: SPACE
SECTION OPA