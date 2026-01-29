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
//<ID> 1252
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const double numbers[] = {1.5, 2.5, 3.5};
    cJSON *root = NULL;
    cJSON *dbl_array = NULL;
    cJSON *retrieved_item = NULL;
    char *printed = NULL;
    char buf[64];
    double second_val = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddNullToObject(root, "missing");
    dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", dbl_array);
    cJSON_AddRawToObject(root, "meta", "{\"created\":true}");

    // step 3: Operate / Validate
    retrieved_item = cJSON_GetArrayItem(dbl_array, 1);
    second_val = cJSON_GetNumberValue(retrieved_item);
    snprintf(buf, sizeof(buf), "%f", second_val);
    cJSON_AddStringToObject(root, "second_value", buf);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}