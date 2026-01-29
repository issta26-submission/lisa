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
//<ID> 1058
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "name", "example");

    cJSON *elem = cJSON_CreateString("elem1");
    cJSON_AddItemToArray(items, elem);

    // step 2: Configure
    cJSON *dup_items = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "dup_items", dup_items);
    cJSON_AddStringToObject(root, "note", "duplicated array");

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_AddStringToObject(root, "serialized", printed);
    cJSON_free(printed);
    void *scratch = cJSON_malloc(16);
    memset(scratch, 0, 16);
    ((char *)scratch)[0] = 'A';
    ((char *)scratch)[1] = 'B';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}