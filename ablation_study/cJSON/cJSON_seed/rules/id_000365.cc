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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *parsed_child = NULL;
    cJSON *parsed_age = NULL;
    cJSON *newobj = NULL;
    double retrieved = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(child, "age", 30.0);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddNumberToObject(root, "rootval", 1.5);
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    parsed = cJSON_Parse(printed);
    parsed_child = cJSON_GetObjectItem(parsed, "child");
    parsed_age = cJSON_GetObjectItem(parsed_child, "age");
    retrieved = cJSON_GetNumberValue(parsed_age);
    buffer[0] = (char)('0' + ((int)retrieved % 10));
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "child");
    newobj = cJSON_CreateObject();
    cJSON_AddNumberToObject(newobj, "x", 5.0);
    cJSON_AddItemToObject(parsed, "newobj", newobj);
    cJSON_AddNumberToObject(parsed, "added", 99.0);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}