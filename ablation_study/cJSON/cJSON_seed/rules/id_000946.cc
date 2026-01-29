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
//<ID> 946
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
    cJSON *label_item = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *s1 = "alpha";
    const char *s2 = "beta";
    const char *label = "MyLabel";
    size_t scratch_len = 64;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item1 = cJSON_CreateString(s1);
    item2 = cJSON_CreateString(s2);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    label_item = cJSON_CreateString(label);
    cJSON_AddItemToObjectCS(root, "label", label_item);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    arr_size = cJSON_GetArraySize(arr);
    scratch[0] = (printed != NULL && printed[0] != '\0') ? printed[0] : ' ';
    scratch[1] = (char)('0' + (arr_size % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}