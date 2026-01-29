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
//<ID> 938
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *item_first = NULL;
    cJSON *item_second = NULL;
    cJSON *item_third = NULL;
    cJSON *lookup_third = NULL;
    char *printed = NULL;
    char *buffer = NULL;
    const char *val_first = "alpha";
    const char *val_second = "beta";
    const char *val_third = "gamma";
    int invalid_flag = 0;
    size_t bufsize = 64;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    item_first = cJSON_AddStringToObject(root, "first", val_first);
    item_second = cJSON_AddStringToObject(root, "second", val_second);
    item_third = cJSON_CreateString(val_third);
    cJSON_AddItemToObject(root, "third", item_third);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    lookup_third = cJSON_GetObjectItem(root, "third");
    invalid_flag = (int)cJSON_IsInvalid(lookup_third);
    cJSON_DeleteItemFromObject(root, "second");
    buffer = (char *)cJSON_malloc(bufsize);
    memset(buffer, 0, bufsize);
    buffer[0] = printed != NULL ? printed[0] : '\0';
    buffer[1] = (char)('0' + (invalid_flag % 10));

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}