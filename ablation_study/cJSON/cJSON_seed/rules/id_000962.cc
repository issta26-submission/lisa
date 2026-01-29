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
//<ID> 962
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *sum_item = NULL;
    cJSON *raw = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    double v1 = 0.0;
    double v2 = 0.0;
    double total = 0.0;
    const char *raw_json = "{\"embedded\":true}";
    size_t scratch_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "values");
    n1 = cJSON_CreateNumber(3.14);
    n2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    raw = cJSON_AddRawToObject(root, "raw", raw_json);
    cJSON_AddItemToObject(root, "constant", cJSON_CreateNumber(42.0));

    // step 3: Operate / Validate
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    total = v1 + v2 + cJSON_GetNumberValue(cJSON_GetObjectItem(root, "constant"));
    sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);
    printed = cJSON_PrintUnformatted(root);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    scratch[0] = printed ? printed[0] : '\0';
    scratch[1] = (char)('0' + (int)total % 10);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}