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
//<ID> 873
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_keep = NULL;
    cJSON *num_remove = NULL;
    cJSON *found_keep = NULL;
    char *dynamic_key = NULL;
    double keep_value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_keep = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "keep", num_keep);
    num_remove = cJSON_CreateNumber(456.0);
    dynamic_key = (char *)cJSON_malloc(16);
    memset(dynamic_key, 0, 16);
    dynamic_key[0] = 'r';
    dynamic_key[1] = 'e';
    dynamic_key[2] = 'm';
    dynamic_key[3] = 'o';
    dynamic_key[4] = 'v';
    dynamic_key[5] = 'e';
    dynamic_key[6] = '\0';
    cJSON_AddItemToObject(root, dynamic_key, num_remove);

    // step 3: Operate / Validate
    found_keep = cJSON_GetObjectItem(root, "keep");
    keep_value = cJSON_GetNumberValue(found_keep);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove");

    // step 4: Cleanup
    cJSON_free(dynamic_key);
    (void)keep_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}