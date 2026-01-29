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
//<ID> 685
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
    char *printed = NULL;
    const char *version = NULL;
    size_t printed_len = 0;
    size_t to_copy = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *stats = cJSON_AddObjectToObject(root, "stats");
    cJSON *num_id = cJSON_CreateNumber(1.0);
    cJSON *num_pi = cJSON_CreateNumber(3.1415);
    cJSON *num_total = cJSON_CreateNumber(100.0);
    cJSON *num_mean = cJSON_CreateNumber(25.5);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(meta, "id", num_id);
    cJSON_AddItemToObject(meta, "pi", num_pi);
    cJSON_AddItemToObject(root, "total", num_total);
    cJSON_AddItemToObject(stats, "mean", num_mean);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    printed_len = printed ? strlen(printed) : 0;
    to_copy = printed_len < 255 ? printed_len : 255;
    memcpy(scratch, printed ? printed : "", to_copy);
    scratch[to_copy] = '\0';
    cJSON_free(printed);
    version = cJSON_Version();
    memcpy(scratch + 128, version ? version : "", strlen(version ? version : ""));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}