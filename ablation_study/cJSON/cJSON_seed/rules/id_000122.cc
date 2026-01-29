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
//<ID> 122
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON *raw = cJSON_CreateRaw("{\"note\":\"raw content\"}");
    cJSON *dup = NULL;
    char *printed = NULL;
    void *tmp_mem = NULL;
    double extracted_value = 0.0;
    cJSON_bool is_raw_flag = 0;
    cJSON_bool compare_equal = 0;

    // step 2: Setup / Configure structure
    cJSON_AddItemToObject(child, "pi", num);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(root, "raw_blob", raw);
    tmp_mem = cJSON_malloc((size_t)64);
    memset(tmp_mem, 0, (size_t)64);

    // step 3: Operate / Validate
    extracted_value = cJSON_GetNumberValue(num);
    cJSON_AddNumberToObject(root, "extracted_pi", extracted_value);
    is_raw_flag = cJSON_IsRaw(raw);
    cJSON_AddNumberToObject(root, "raw_flag", (double)is_raw_flag);
    dup = cJSON_Duplicate(root, 1);
    compare_equal = cJSON_Compare(root, dup, 1);
    cJSON_AddNumberToObject(root, "compare_equal", (double)compare_equal);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_mem);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}