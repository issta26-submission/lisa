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
//<ID> 1417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const double numbers[] = {4.2, 5.3, 6.4};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "data", dbl_arr);
    const char *version_str = cJSON_Version();
    cJSON *version_ref = cJSON_CreateStringReference(version_str);
    cJSON_AddItemToObject(root, "version", version_ref);
    cJSON_AddNumberToObject(root, "threshold", 3.14159);

    // step 2: Configure
    cJSON *data_copy = cJSON_Duplicate(dbl_arr, 1);
    cJSON_AddItemToObject(root, "data_copy", data_copy);

    // step 3: Operate & Validate
    int data_size = cJSON_GetArraySize(dbl_arr);
    cJSON *first_elem = cJSON_GetArrayItem(dbl_arr, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    (void)data_size;
    (void)first_value;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}