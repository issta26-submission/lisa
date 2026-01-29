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
//<ID> 1912
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
    cJSON *num_item = NULL;
    cJSON *got_num = NULL;
    char *out = NULL;
    char *sv = NULL;
    double num_val = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "value", num_item);

    // step 3: Core operations
    sv = cJSON_SetValuestring(num_item, "one-two-three");
    out = cJSON_PrintUnformatted(root);
    got_num = cJSON_GetObjectItem(root, "value");
    num_val = cJSON_GetNumberValue(got_num);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(sv != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(num_val == 123.456);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}