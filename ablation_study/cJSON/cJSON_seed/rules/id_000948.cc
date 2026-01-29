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
//<ID> 948
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
    cJSON *name_cs = NULL;
    cJSON *name = NULL;
    cJSON *item1 = NULL;
    cJSON *item2 = NULL;
    char *printed = NULL;
    char *scratch = NULL;
    const char *text_cs = "CaseSensitiveName";
    const char *text = "regularName";
    const char *a = "one";
    const char *b = "two";
    size_t scratch_len = 8;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    name_cs = cJSON_CreateString(text_cs);
    cJSON_AddItemToObjectCS(root, "NameCS", name_cs);
    name = cJSON_CreateString(text);
    cJSON_AddItemToObject(root, "name", name);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item1 = cJSON_CreateString(a);
    item2 = cJSON_CreateString(b);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    scratch[0] = (printed && printed[0]) ? printed[0] : '\0';
    scratch[1] = (printed && printed[1]) ? printed[1] : '\0';
    scratch[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}