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
//<ID> 1289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *direct_num_item = NULL;
    cJSON *alias_item = NULL;
    char *printed = NULL;
    double alias_value = 0.0;
    double direct_value = 0.0;
    cJSON_bool ref_added = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "primary_number", num_item);
    direct_num_item = cJSON_AddNumberToObject(root, "direct_number", 7.5);
    cJSON_AddNullToObject(root, "nothing_here");
    ref_added = cJSON_AddItemReferenceToObject(root, "primary_alias", num_item);

    // step 3: Operate / Validate
    alias_item = cJSON_GetObjectItem(root, "primary_alias");
    alias_value = cJSON_GetNumberValue(alias_item);
    direct_value = cJSON_GetNumberValue(direct_num_item);
    printed = cJSON_PrintUnformatted(root);
    (void)ref_added;
    (void)alias_value;
    (void)direct_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}