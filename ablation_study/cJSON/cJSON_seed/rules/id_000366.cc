#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *age_item = NULL;
    double age_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "person", person);
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddNumberToObject(root, "age", 30.5);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    age_item = cJSON_GetObjectItem(parsed, "age");
    age_val = cJSON_GetNumberValue(age_item);
    buffer[0] = printed ? printed[0] : '\0';
    buffer[1] = (char)('0' + (int)(age_val) % 10);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "age");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}