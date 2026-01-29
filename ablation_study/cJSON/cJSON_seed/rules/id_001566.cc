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
//<ID> 1566
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *trueItem = NULL;
    cJSON *retrieved = NULL;
    cJSON *copied_str = NULL;
    char *printed = NULL;
    char *copy_buf = NULL;
    const char *first_val = NULL;
    size_t len = 0;
    cJSON_bool is_string = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    trueItem = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", trueItem);

    // step 3: Operate / Validate
    retrieved = cJSON_GetArrayItem(arr, 0);
    is_string = cJSON_IsString(retrieved);
    first_val = cJSON_GetStringValue(retrieved);
    len = strlen(first_val) + 1;
    copy_buf = (char *)cJSON_malloc(len);
    memset(copy_buf, 0, len);
    memcpy(copy_buf, first_val, len);
    copied_str = cJSON_CreateString(copy_buf);
    cJSON_AddItemToObject(root, "copied_first", copied_str);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(copy_buf);
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}