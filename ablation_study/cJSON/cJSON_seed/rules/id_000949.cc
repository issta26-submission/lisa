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
//<ID> 949
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *items = NULL;
    cJSON *s1 = NULL;
    cJSON *s2 = NULL;
    cJSON *s3 = NULL;
    cJSON *name_item = NULL;
    cJSON *dup = NULL;
    const char *first_item_str = NULL;
    char *printed = NULL;
    char *printed_dup = NULL;
    char *scratch = NULL;
    int items_count = 0;
    const char *name_text = "example";
    const char *a = "one";
    const char *b = "two";
    const char *c3 = "three";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    items = cJSON_CreateArray();
    s1 = cJSON_CreateString(a);
    s2 = cJSON_CreateString(b);
    s3 = cJSON_CreateString(c3);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToArray(items, s3);
    cJSON_AddItemToObjectCS(root, "items", items);
    name_item = cJSON_CreateString(name_text);
    cJSON_AddItemToObject(root, "name", name_item);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    dup = cJSON_Duplicate(root, 1);
    printed_dup = cJSON_Print(dup);
    items_count = cJSON_GetArraySize(cJSON_GetObjectItem(root, "items"));
    first_item_str = cJSON_GetStringValue(cJSON_GetArrayItem(cJSON_GetObjectItem(root, "items"), 0));
    scratch = (char *)cJSON_malloc((size_t)16);
    memset(scratch, 0, (size_t)16);
    scratch[0] = first_item_str[0];
    scratch[1] = (char)('0' + (items_count % 10));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(dup);

    // API sequence test completed successfully
    return 66;
}