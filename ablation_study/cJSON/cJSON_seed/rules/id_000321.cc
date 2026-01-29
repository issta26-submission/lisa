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
//<ID> 321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char json_text[] = "{\"alpha\":1.5,\"beta\":2}";
    size_t json_len = sizeof(json_text) - 1;
    cJSON *parsed = cJSON_ParseWithLength(json_text, json_len);
    cJSON *root = cJSON_CreateObject();
    cJSON *created_num = cJSON_CreateNumber(3.14);
    cJSON *added_num = cJSON_AddNumberToObject(root, "added", 7.77);
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    double val_alpha = 0.0;
    double val_added = 0.0;
    cJSON *item_alpha = NULL;
    cJSON *item_added = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "created", created_num);
    cJSON_AddNumberToObject(root, "beta_copy", 2.0);

    // step 3: Operate / Validate
    item_alpha = cJSON_GetObjectItem(parsed, "alpha");
    val_alpha = cJSON_GetNumberValue(item_alpha);
    item_added = cJSON_GetObjectItem(root, "added");
    val_added = cJSON_GetNumberValue(added_num);
    printed = cJSON_PrintUnformatted(root);
    buffer[0] = (char)('A' + ((int)val_alpha % 26));
    buffer[1] = (char)('A' + ((int)val_added % 26));

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    return 66;
    // API sequence test completed successfully
}