### Para inicializar los submodulos:

`git submodule update --init --recursive --remote`

Esto es necesario la primera vez (cuando el submodulo no está inicializado en tu local).
También se utiliza para cambiar el commit al que apunta el submodulo al mas reciente de la rama.

### Configuracion para que en tu ordenador cada vez que hagas pull actualice submodulos

`git config submodule.recurse true`

Esto NO LOS INICIALIZA entonces el primer comando hay que ejecutarlo mínimo una vez