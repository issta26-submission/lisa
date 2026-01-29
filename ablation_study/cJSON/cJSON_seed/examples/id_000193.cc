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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToArray(array, raw_item);
    cJSON *str_item = cJSON_CreateString("element_one");
    cJSON_AddItemToArray(array, str_item);
    cJSON *title = cJSON_CreateString("example_title");
    cJSON_AddItemToObject(root, "title", title);

    // step 3: Operate
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    char *printed = cJSON_PrintUnformatted(root);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), raw_flag);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}