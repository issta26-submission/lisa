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
//<ID> 914
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *names = NULL;
    cJSON *info = NULL;
    cJSON *name0 = NULL;
    cJSON *name1 = NULL;
    cJSON *retrieved_names = NULL;
    char *printed = NULL;
    char *buffer = NULL;
    int names_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    names = cJSON_CreateArray();
    name0 = cJSON_CreateStringReference("Alice");
    name1 = cJSON_CreateStringReference("Bob");
    cJSON_AddItemToArray(names, name0);
    cJSON_AddItemToArray(names, name1);
    cJSON_AddItemToObject(root, "names", names);
    info = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddStringToObject(info, "note", "sample");

    // step 3: Operate / Validate
    retrieved_names = cJSON_GetObjectItemCaseSensitive(root, "names");
    names_size = cJSON_GetArraySize(retrieved_names);
    printed = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "info");
    buffer = (char *)cJSON_malloc(8);
    memset(buffer, 0, 8);
    buffer[0] = (char)('0' + (names_size % 10));
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)name0;
    (void)name1;
    (void)retrieved_names;
    (void)info;
    return 66;
}