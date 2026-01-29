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
//<ID> 1396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    float fnums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *farray = cJSON_CreateFloatArray(fnums, 3);
    double dnums[4] = {4.0, 5.0, 6.0, 7.0};
    cJSON *darray = cJSON_CreateDoubleArray(dnums, 4);
    cJSON_AddItemToObject(root, "values", farray);
    cJSON_AddItemToObject(root, "meta", cJSON_CreateString("original"));

    // step 2: Configure
    cJSON_ReplaceItemInObject(root, "values", darray);
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());

    // step 3: Operate
    cJSON *values_item = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *meta_item = cJSON_GetObjectItemCaseSensitive(root, "meta");
    int value_count = cJSON_GetArraySize(values_item);
    cJSON *first_value = cJSON_GetArrayItem(values_item, 0);
    double first_num = cJSON_GetNumberValue(first_value);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}