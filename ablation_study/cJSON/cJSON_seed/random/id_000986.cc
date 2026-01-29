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
//<ID> 986
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *meta_key = "meta";
    const char *num_key = "count";
    const double original_number = 12345.678;
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *added_num_item = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON *fetched_meta = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    double fetched_value = 0.0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root and a nested object to hold the number
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();

    // step 3: Configure - create and attach a number to the nested object, then attach that nested object to root
    added_num_item = cJSON_AddNumberToObject(meta, num_key, original_number);
    add_ok = cJSON_AddItemToObject(root, meta_key, meta);

    // step 4: Operate - retrieve the nested object and then retrieve the number item (case-sensitive and insensitive access)
    fetched_meta = cJSON_GetObjectItem(root, meta_key);
    fetched_num = cJSON_GetObjectItemCaseSensitive(fetched_meta, num_key);

    // step 5: Validate - serialize root to exercise allocator, extract numeric value and compute a simple validation score
    serialized = cJSON_Print(root);
    fetched_value = (fetched_num != (cJSON *)0) ? cJSON_GetNumberValue(fetched_num) : 0.0;
    validation_score = (int)(root != (cJSON *)0) + (int)(meta == fetched_meta) + (int)(added_num_item != (cJSON *)0) + (int)(add_ok) + (int)(serialized != (char *)0) + (int)(fetched_value == original_number);
    (void)validation_score;

    // step 6: Cleanup - free serialized buffer and delete the root (which frees attached items)
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}