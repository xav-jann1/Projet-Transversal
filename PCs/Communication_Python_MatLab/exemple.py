# Exemple pour récupérer les résultats obtenus par MatLab:

import matlab.engine
eng = matlab.engine.start_matlab()

eng.hello(nargout=0)
x = eng.eval('x')

print x
print type(x)
eng.quit()