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
//<ID> 103
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON *num = cJSON_CreateNumber(99.5);
    char *buffer = (char *)cJSON_malloc(512);
    cJSON_bool print_ok = 0;
    cJSON *got_arr = NULL;
    cJSON *got_obj = NULL;
    cJSON *got_num = NULL;
    cJSON *got_str = NULL;
    double got_num_value = 0.0;
    const char *got_str_value = NULL;
    cJSON_bool is_str = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(obj, "value", num);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "object", obj);
    cJSON_AddItemToObject(root, "greeting", cJSON_CreateString("hello"));

    // step 3: Operate / Validate
    got_arr = cJSON_GetObjectItem(root, "array");
    got_obj = cJSON_GetObjectItemCaseSensitive(root, "object");
    got_num = cJSON_GetObjectItem(got_obj, "value");
    got_str = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    got_num_value = cJSON_GetNumberValue(got_num);
    got_str_value = cJSON_GetStringValue(got_str);
    is_str = cJSON_IsString(got_str);
    print_ok = cJSON_PrintPreallocated(root, buffer, 512, 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}