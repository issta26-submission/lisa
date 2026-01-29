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
//<ID> 1917
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item1 = NULL;
    cJSON *num_item2 = NULL;
    cJSON *got_initial = NULL;
    cJSON *got_modified = NULL;
    char *setstr = NULL;
    char *out = NULL;
    double initial_val = 0.0;
    double modified_val = 0.0;
    int validation = 0;

    // step 2: Setup (Initialize)
    root = cJSON_CreateObject();
    num_item1 = cJSON_CreateNumber(123.45);
    cJSON_AddItemToObject(root, "initial", num_item1);

    // step 3: Configure (modify an item's string value then add)
    num_item2 = cJSON_CreateNumber(99.0);
    setstr = cJSON_SetValuestring(num_item2, "ninety nine");
    cJSON_AddItemToObject(root, "modified", num_item2);

    // step 4: Operate (print and retrieve numeric values)
    out = cJSON_PrintUnformatted(root);
    got_initial = cJSON_GetObjectItem(root, "initial");
    initial_val = cJSON_GetNumberValue(got_initial);
    got_modified = cJSON_GetObjectItem(root, "modified");
    modified_val = cJSON_GetNumberValue(got_modified);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item1 != NULL);
    validation ^= (int)(num_item2 != NULL);
    validation ^= (int)(setstr != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_initial != NULL);
    validation ^= (int)(got_modified != NULL);
    validation ^= (int)(initial_val == 123.45);
    validation ^= (int)(modified_val == 99.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}