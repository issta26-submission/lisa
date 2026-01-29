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
//<ID> 677
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *copy = NULL;
    cJSON *other = NULL;
    cJSON *name_item = NULL;
    cJSON_bool added_meta = 0;
    cJSON_bool equal_same = 0;
    cJSON_bool equal_diff = 0;

    // step 2: Setup / Configure
    name_item = cJSON_AddStringToObject(root, "name", "example_name");
    cJSON_AddStringToObject(meta, "version", "1.0");
    added_meta = cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    copy = cJSON_Duplicate(root, 1);
    equal_same = cJSON_Compare(root, copy, 1);
    other = cJSON_CreateObject();
    cJSON_AddStringToObject(other, "name", "different_name");
    equal_diff = cJSON_Compare(root, other, 1);

    // step 4: Cleanup
    cJSON_Delete(copy);
    cJSON_Delete(other);
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}