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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    char *json_text = (char *)0;
    cJSON_bool arr_check = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    raw_item = cJSON_AddRawToObject(root, "metadata", "{\"created\":true,\"count\":0}");

    // step 4: Operate
    arr_check = cJSON_IsArray(arr);
    json_text = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)arr_check;
    (void)raw_item;
    (void)json_text;

    // step 6: Cleanup
    cJSON_free(json_text);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}