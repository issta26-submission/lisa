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
//<ID> 755
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    const char *json_text = "{\"numbers\": [1, 2]}";
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *new_obj = NULL;
    cJSON *node_num = NULL;
    char *printed = NULL;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_ParseWithOpts(json_text, &parse_end, 1);
    arr = cJSON_GetObjectItem(root, "numbers");
    size_before = cJSON_GetArraySize(arr);

    // step 3: Operate / Validate
    new_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_obj, "added", 42.0);
    cJSON_AddItemToArray(arr, new_obj);
    node_num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(arr, node_num);
    cJSON_AddItemToObject(root, "extra", cJSON_CreateString("note"));
    printed = cJSON_PrintUnformatted(root);
    memcpy(scratch, printed, strlen(printed) + 1);
    size_after = cJSON_GetArraySize(arr);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}