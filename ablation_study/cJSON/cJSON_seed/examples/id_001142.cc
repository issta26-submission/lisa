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
//<ID> 1142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw1[] = "{\"items\":[1,2,3],\"meta\":{\"name\":\"test\"}}";
    const char raw2[] = "{\"other\":[10,20]}";
    size_t len1 = sizeof(raw1) - 1;
    cJSON *root1 = cJSON_ParseWithLength(raw1, len1);
    cJSON *root2 = cJSON_Parse(raw2);

    // step 2: Configure
    cJSON *items_array = cJSON_GetObjectItem(root1, "items");
    cJSON *second_item = cJSON_GetArrayItem(items_array, 1);
    double extracted_value = cJSON_GetNumberValue(second_item);
    cJSON *imported_items = cJSON_Duplicate(items_array, 1);
    cJSON *nullable = cJSON_CreateNull();
    cJSON *extracted_value_node = cJSON_CreateNumber(extracted_value);
    cJSON_AddItemToObject(root2, "imported_items", imported_items);
    cJSON_AddItemToObject(root2, "nullable", nullable);
    cJSON_AddItemToObject(root2, "extracted_value", extracted_value_node);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root2);
    (void)out;

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root2);
    cJSON_Delete(root1);
    // API sequence test completed successfully
    return 66;
}