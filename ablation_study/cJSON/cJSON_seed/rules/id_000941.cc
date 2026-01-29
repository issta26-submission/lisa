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
//<ID> 941
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
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    cJSON *retrieved = NULL;
    const char *s1 = "alpha";
    const char *s2 = "beta";
    const char *name_const = "const_name";
    char *printed = NULL;
    char *scratch = NULL;
    const char *retrieved_str = NULL;
    int arrsz = 0;
    int scratch_len = 64;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString(s1);
    item2 = cJSON_CreateString(s2);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "my_array", arr);
    cJSON_AddItemToObjectCS(root, name_const, cJSON_CreateString("const_value"));
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("example_label"));

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    arrsz = cJSON_GetArraySize(arr);
    retrieved = cJSON_GetArrayItem(arr, 0);
    retrieved_str = cJSON_GetStringValue(retrieved);
    scratch = (char *)cJSON_malloc((size_t)scratch_len);
    memset(scratch, 0, (size_t)scratch_len);
    scratch[0] = (retrieved_str != NULL && retrieved_str[0] != '\0') ? retrieved_str[0] : ' ';
    scratch[1] = (char)('0' + (arrsz % 10));
    scratch[2] = (printed != NULL && printed[0] != '\0') ? printed[0] : ' ';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}