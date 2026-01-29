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
//<ID> 1852
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
    cJSON *num = NULL;
    cJSON *fitem = NULL;
    cJSON *retrieved = NULL;
    char *out = NULL;
    double got = 0.0;
    int validation = 0;
    cJSON_bool added_num = 0;
    cJSON_bool added_false = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    added_num = cJSON_AddItemToObject(root, "answer", num);
    fitem = cJSON_CreateFalse();
    added_false = cJSON_AddItemToObject(root, "flag", fitem);

    // step 3: Core operations
    out = cJSON_PrintUnformatted(root);
    retrieved = cJSON_GetObjectItem(root, "answer");
    got = cJSON_GetNumberValue(retrieved);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(fitem != NULL);
    validation ^= (int)(retrieved != NULL);
    validation ^= (int)(got == 42.0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(added_num != 0);
    validation ^= (int)(added_false != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}