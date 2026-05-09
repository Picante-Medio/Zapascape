### Para actualizar los submodulos:

`git submodule update --init --recursive`

Esto es necesario la primera vez (cuando el submodulo no está inicializado en tu local)

### Configuracion para que en tu ordenador cada vez que hagas pull actualice submodulos

`git config submodule.recurse true`

Esto NO LOS INICIALIZA entonces el primer comando hay que ejecutarlo mínimo una vez