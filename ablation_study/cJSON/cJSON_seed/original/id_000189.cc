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
//<ID> 189
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
    cJSON_AddItemToObject(root, "data", array);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("example");
    cJSON_AddItemToArray(array, num);
    cJSON_AddItemToArray(array, str);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(array, null_item);

    // step 2: Configure
    char *unformatted = cJSON_PrintUnformatted(root);
    cJSON_Minify(unformatted);
    void *tmp_buf = cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    cJSON *parsed = cJSON_Parse(unformatted);

    // step 3: Operate and Validate
    int array_size = cJSON_GetArraySize(array);
    cJSON *first_elem = cJSON_GetArrayItem(array, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    int summary = array_size + (int)first_val + (int)equal + root->type + parsed->type;
    (void)summary;

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(tmp_buf);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}