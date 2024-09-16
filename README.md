**Mortal C-ombat**

1. **Structures:**
   - `Node`: Used to store the history of each player's attacks.
   - `Jogador`: Represents a player, including their health, stamina, and attack history.
   - `Combo`: Represents a combination move, containing the sequence of attacks, its damage, stamina cost, and a unique identifier.

2. **Core Functions:**
   - `gerador`: Generates a random number between two values.
   - `limpar_historico`: Clears the attack history of a player.
   - `adicionar_historico`: Adds an attack or combo to the player's history.
   - `executar_combo`: Checks if a combo is valid, and if so, applies its effects (damages the opponent and reduces stamina).
   - `comboespecial`: Implements a special combo move, which appears to allow for a "rollback" of attacks.
   - `processar_ataques`: Processes each player's attack round by round, comparing moves and calculating damage based on a predefined table.
   - `cheatcodes`: Allows players to input cheat codes to adjust health, stamina, or reset the game state.

3. **Special Features:**
   - **Combo System**: Players can execute powerful combos if they match specific sequences of moves. Combos have a higher damage output but also drain stamina.
   - **Attack Comparison**: Attacks are compared using a 2D array (`tabela`) that defines how different moves interact with each other, adding an element of strategy.
   - **Computer Logic**: The computer opponent selects its moves based on the player's history and might trigger a combo or special attack after a certain threshold.

4. **Cheat Codes**:
   - `Modo-Jesus`: Resets both players' health and stamina.
   - `Alt-F4`, `Kebab`, `Hiroshima`, `Nood-Mode`: Various codes for restoring or modifying stamina and health.

5. **Edge Cases:**
   - The code has robust handling for edge cases like invalid inputs, ensuring that combos and attacks are checked before execution.
   - `comboespecial` allows players to roll back a number of previous moves, effectively undoing part of the game's history.

Overall, this code represents a dynamic game loop where the outcome of a round depends on attack comparisons, special moves, and potentially using cheat codes. The `imprimir_historico` and related history functions ensure that the game maintains a log of past actions for both gameplay tracking and potential rewinds via special combos.
