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
//<ID> 956
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *array = NULL;
    cJSON *first = NULL;
    cJSON *second = NULL;
    cJSON *inserted = NULL;
    cJSON *retrieved_array = NULL;
    cJSON *retrieved_item = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *s1 = "alpha";
    const char *s2 = "beta";
    const char *insert_text = "middle";
    const char *retrieved_str = NULL;
    cJSON_bool cmp_res = 0;
    int scratch_len = 16;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    first = cJSON_CreateString(s1);
    second = cJSON_CreateString(s2);
    cJSON_AddItemToArray(array, first);
    cJSON_AddItemToArray(array, second);
    cJSON_AddItemToObject(root, "letters", array);
    inserted = cJSON_CreateString(insert_text);
    cJSON_InsertItemInArray(array, 1, inserted);

    // step 3: Operate / Validate
    retrieved_array = cJSON_GetObjectItem(root, "letters");
    retrieved_item = cJSON_GetArrayItem(retrieved_array, 1);
    retrieved_str = cJSON_GetStringValue(retrieved_item);
    cmp_res = cJSON_Compare(first, inserted, 1);
    printed = cJSON_PrintUnformatted(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = retrieved_str[0];
    scratch[1] = (char)('0' + (int)cmp_res);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}