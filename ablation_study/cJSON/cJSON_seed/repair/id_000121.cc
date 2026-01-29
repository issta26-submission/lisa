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
//<ID> 121
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *placeholder = cJSON_AddArrayToObject(root, "values");
    cJSON_AddStringToObject(root, "title", "demo");
    cJSON *parsed = cJSON_Parse("{\"meta\":\"imported\",\"count\":7}");
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta");
    const char *meta_str = cJSON_GetStringValue(parsed_meta);
    cJSON_AddStringToObject(root, "imported_meta", meta_str);

    // step 2: Configure
    double numbers[4];
    numbers[0] = 3.14;
    numbers[1] = 1.618;
    numbers[2] = 2.718;
    numbers[3] = 0.5772;
    cJSON *doubleArray = cJSON_CreateDoubleArray(numbers, 4);
    cJSON_ReplaceItemViaPointer(root, placeholder, doubleArray);

    // step 3: Operate and Validate
    int array_size = cJSON_GetArraySize(doubleArray);
    cJSON_AddNumberToObject(root, "count", (double)array_size);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}