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
//<ID> 2061
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
    cJSON *got_alpha = NULL;
    cJSON *got_beta = NULL;
    char *out_unformatted = NULL;
    char *out_buffered = NULL;
    double val_alpha = 0.0;
    double val_beta = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item1 = cJSON_AddNumberToObject(root, "alpha", 3.14);
    num_item2 = cJSON_AddNumberToObject(root, "beta", -42.0);

    // step 3: Operate
    got_alpha = cJSON_GetObjectItem(root, "alpha");
    got_beta = cJSON_GetObjectItem(root, "beta");
    val_alpha = cJSON_GetNumberValue(got_alpha);
    val_beta = cJSON_GetNumberValue(got_beta);

    // step 4: Serialization
    out_unformatted = cJSON_PrintUnformatted(root);
    out_buffered = cJSON_PrintBuffered(root, 128, 1);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item1 != NULL);
    validation ^= (int)(num_item2 != NULL);
    validation ^= (int)(got_alpha != NULL);
    validation ^= (int)(got_beta != NULL);
    validation ^= (int)(out_unformatted != NULL);
    validation ^= (int)(out_buffered != NULL);
    validation ^= (int)(val_alpha > 0.0);
    validation ^= (int)(val_beta < 0.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    if (out_unformatted) { cJSON_free(out_unformatted); }
    if (out_buffered) { cJSON_free(out_buffered); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}