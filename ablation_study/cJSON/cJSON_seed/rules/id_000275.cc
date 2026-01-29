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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.25);
    cJSON *n2 = cJSON_CreateNumber(3.75);
    int size = 0;
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    char *json = NULL;
    cJSON *minified_str = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 3: Operate / Validate
    size = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    v1 = cJSON_GetNumberValue(item0);
    v2 = cJSON_GetNumberValue(item1);
    sum = v1 + v2;
    cJSON_AddItemToObject(root, "sum", cJSON_CreateNumber(sum));
    json = cJSON_PrintUnformatted(root);
    cJSON_Minify(json);
    minified_str = cJSON_CreateString(json);
    cJSON_AddItemToObject(root, "minified", minified_str);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}