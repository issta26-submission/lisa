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
//<ID> 126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *num1 = cJSON_CreateNumber(3.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON *raw = cJSON_CreateRaw("{\"x\":1}");
    cJSON *dup = NULL;
    cJSON *a_item = NULL;
    cJSON *b_item = NULL;
    cJSON *raw_item = NULL;
    char *printed = NULL;
    void *tmp_mem = NULL;
    double total = 0.0;
    cJSON_bool is_raw = 0;
    cJSON_bool cmp_equal = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddItemToObject(metrics, "a", num1);
    cJSON_AddItemToObject(metrics, "b", num2);
    cJSON_AddItemToObject(root, "raw_blob", raw);
    tmp_mem = cJSON_malloc((size_t)16);
    memset(tmp_mem, 0, 16);

    // step 3: Operate / Validate
    a_item = cJSON_GetObjectItem(metrics, "a");
    b_item = cJSON_GetObjectItem(metrics, "b");
    total = cJSON_GetNumberValue(a_item) + cJSON_GetNumberValue(b_item);
    cJSON_AddNumberToObject(root, "total", total);
    raw_item = cJSON_GetObjectItem(root, "raw_blob");
    is_raw = cJSON_IsRaw(raw_item);
    dup = cJSON_Duplicate(root, 1);
    cmp_equal = cJSON_Compare(root, dup, 1);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_mem);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}