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
//<ID> 1192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char sample_json[] = "  { \"unused\" : [ 1, 2, 3 ] }  ";
    cJSON_Minify(sample_json);

    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);

    cJSON *str_first = cJSON_CreateString("first");
    cJSON_AddItemToArray(arr, str_first);
    cJSON *bool_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, bool_false);
    cJSON *str_third = cJSON_CreateString("third");
    cJSON_AddItemToArray(arr, str_third);

    // step 2: Configure
    int before_count = cJSON_GetArraySize(arr);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    const char *second_as_string = cJSON_GetStringValue(second_item); (void)second_as_string;

    // step 3: Operate
    cJSON_DeleteItemFromArray(arr, 1);
    int after_count = cJSON_GetArraySize(arr);
    cJSON *copied = cJSON_Duplicate(root, 1);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    (void)before_count;
    (void)after_count;
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(copied);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}