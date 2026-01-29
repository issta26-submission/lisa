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
//<ID> 930
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *alt_item = NULL;
    cJSON *retrieved = NULL;
    char *printed1 = NULL;
    char *printed2 = NULL;
    char *buf = NULL;
    const char *primary_name = "Alice";
    const char *alternate_name = "Bob";
    cJSON_bool is_invalid = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", primary_name);
    name_item = cJSON_CreateString(alternate_name);
    cJSON_AddItemToObject(root, "alternate", name_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "name");
    is_invalid = cJSON_IsInvalid(retrieved);
    printed1 = cJSON_PrintUnformatted(root);
    cJSON_DeleteItemFromObject(root, "name");
    printed2 = cJSON_PrintUnformatted(root);
    buf = (char *)cJSON_malloc(16);
    memset(buf, 0, 16);
    buf[0] = (printed1 != NULL && printed1[0] != '\0') ? printed1[0] : ' ';
    buf[1] = (char)('0' + (int)(is_invalid % 10));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}