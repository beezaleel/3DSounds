# 3DSounds
### How to build
1. Right click on project and select build.
2. Click `Debug -> Start Debugging` to run
#### Keyboard Functions
1. Camera view is enabled by default.
2. Press letter `A` to move x-axis left
3. Press letter `D` to move x-axis right
4. Press letter `E` to move up
5. Press letter `Q` to move down
6. Press letter `W` to move z-axis up
7. Press letter `S` to move z-axis down

### Speaker locations
1. Front Left - glm::vec4(398.0f, 30.0f, 566.0f, 1);
2. Front Right - glm::vec4(-526, 30.0f, 470, 1);
3. Rare Right - glm::vec4(-568, 30.0f, -514, 1);
4. Rare Left - glm::vec4(-112, 30.0f, -550, 1);
5. Center - glm::vec4(-174, 30.0f, -130, 1);

### Quick jump to speaker locations
1. Press `R` to jump to Front Left speaker's location
2. Press `T` to jump to Front Right speaker's location
3. Press `Y` to jump to Rare Right speaker's location
4. Press `U` to jump to Rare Left speaker's location
5. Press `I` to jump to Center speaker's location
6. Press `O` to jump to back start position

#### Initial Camera start position
glm::vec4(541, 20, -520, 1);