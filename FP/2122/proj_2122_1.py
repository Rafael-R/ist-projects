


def corrigir_palavra(word):
    length = len(word)
    i = 0
    while(i != length - 1):
        pair = word[i : i + 2]
        if abs(ord(pair[0]) - ord(pair[1])) == 32:
            word = word[:i] + word[i + 2:]
            length = len(word)
            i = 0
        else:
            i += 1
    return word


def eh_anagrama(original, word):
    original = original.lower()
    word = word.lower()
    length = len(original)
    if length == len(word):
        i = 0
        while(i < length):
            if original[i] in word:
                word = word.replace(original[i], "", 1)
            i += 1
        if word == "":
            return True
    return False


def corrigir_doc(text):
    words = text.split(" ")
    corrected = [""]
    for word in words:
        if word.isalpha():
            word = corrigir_palavra(word)
            error = 0;
            for c in corrected:
                if eh_anagrama(c, word) is True and c.lower() != word.lower():
                    error += 1
            if error == 0:
                corrected += [word]
        else:
            raise ValueError("corrigir_doc: argumento invalido")
    res = ''
    for c in corrected[1:]:
        res += c + ' '
    return res


print(corrigir_palavra('abBAx'))
print(corrigir_palavra('cCdatabasacCADde'))

print(eh_anagrama('caso', 'SaCo'))
print(eh_anagrama('caso', 'casos'))

doc = "BuAaXOoxiIKoOkggyrFfhHXxR duJjUTtaCcmMtaAGga \
eEMmtxXOjUuJQqQHhqoada JlLjbaoOsuUeYy cChgGvValLCwMmWBbclLsNn \
LyYlMmwmMrRrongTtoOkyYcCK daRfFKkLlhHrtZKqQkkvVKza"
print(corrigir_doc(doc))
doc = 'data outra daTA outra TADA'
print(corrigir_doc(doc))