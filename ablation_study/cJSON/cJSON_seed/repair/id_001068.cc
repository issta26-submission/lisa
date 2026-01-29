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
//<ID> 1068
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *elements = cJSON_AddArrayToObject(root, "elements");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *always_true = cJSON_AddTrueToObject(root, "always_true");
    (void)always_true;

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(elements, num1);
    cJSON_AddItemToArray(elements, num2);
    cJSON *name = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON *meta_flag = cJSON_CreateBool(meta_is_obj);
    cJSON_AddItemToObject(root, "meta_is_object", meta_flag);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(elements);
    cJSON *first = cJSON_GetArrayItem(elements, 0);
    double first_num = cJSON_GetNumberValue(first);
    (void)first_num;
    char *flat = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(32);
    memset(buffer, 0, 32);
    ((char *)buffer)[0] = (char)('0' + (count & 0xF));
    ((char *)buffer)[1] = (char)('0' + (meta_is_obj ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(flat);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}