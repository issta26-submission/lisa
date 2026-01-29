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
//<ID> 1573
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"a\":123,\"b\":123,\"arr\":[1,2,3]}";
    cJSON *root = NULL;
    cJSON *item_a = NULL;
    cJSON *item_b = NULL;
    cJSON *arr = NULL;
    cJSON *first = NULL;
    char *printed = NULL;
    cJSON_bool equal = 0;
    double a_value = 0.0;
    double first_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    item_a = cJSON_GetObjectItem(root, "a");
    item_b = cJSON_GetObjectItem(root, "b");
    arr = cJSON_GetObjectItem(root, "arr");
    first = cJSON_GetArrayItem(arr, 0);
    printed = cJSON_PrintUnformatted(root);

    // step 3: Operate / Validate
    equal = cJSON_Compare(item_a, item_b, 1);
    a_value = cJSON_GetNumberValue(item_a);
    first_value = cJSON_GetNumberValue(first);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}