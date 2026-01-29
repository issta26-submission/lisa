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
//<ID> 940
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
    cJSON *name_item = NULL;
    cJSON *hobby1 = NULL;
    cJSON *hobby2 = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *name_text = "Alice";
    const char *hobby_text1 = "reading";
    const char *hobby_text2 = "chess";
    const char *retrieved_name = NULL;
    int scratch_len = 32;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    name_item = cJSON_CreateString(name_text);
    cJSON_AddItemToObject(root, "name", name_item);
    hobby1 = cJSON_CreateString(hobby_text1);
    hobby2 = cJSON_CreateString(hobby_text2);
    cJSON_AddItemToArray(arr, hobby1);
    cJSON_AddItemToArray(arr, hobby2);
    cJSON_AddItemToObjectCS(root, "hobbies", arr);

    // step 3: Operate / Validate
    retrieved_name = cJSON_GetStringValue(cJSON_GetObjectItem(root, "name"));
    printed = cJSON_Print(root);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = retrieved_name[0];
    scratch[1] = printed[0];
    scratch[2] = (char)('0' + (cJSON_GetArraySize(arr) % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}